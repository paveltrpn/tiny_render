
#include <fstream>
#include <algorithm>
#include <vector>
#include <vulkan/vk_enum_string_helper.h>

#include "shader_storage.h"
#include "../../../log/log.h"

static constexpr bool DEBUG_OUTPUT_SHADER_STORAGE_VK_CPP{ true };

namespace tire::vk {

ShaderStorage::ShaderStorage( const vk::Device *device )
    : device_{ device } {
}

ShaderStorage::~ShaderStorage() {
    for ( const auto &module : modules_ ) {
        vkDestroyShaderModule( device_->handle(), std::get<1>( module ),
                               nullptr );
    }
}

void ShaderStorage::push( std::span<uint8_t> bytecode,
                          const std::string &name ) {
    // Create vulkan shader module
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = bytecode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>( bytecode.data() );

    VkShaderModule module;
    if ( const auto err = vkCreateShaderModule( device_->handle(), &createInfo,
                                                nullptr, &module );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create shader module {} with code {}!",
                         name, string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_SHADER_STORAGE_VK_CPP>(
            "vk::ShaderStorage == shader module {} created!", name );
    }

    modules_[name] = module;
}

void ShaderStorage::add( const std::filesystem::path &path ) {
    if ( device_->handle() == VK_NULL_HANDLE ) {
        throw std::runtime_error( std::format(
            "can't use shaders before valid logical device is acquired!" ) );
    }

    std::ifstream file( path, std::ios::ate | std::ios::binary );
    if ( !file.is_open() ) {
        throw std::runtime_error(
            std::format( "failed to open file {}!", path.string() ) );
    }

    // Future shader name, comes from filename. Have format:
    // "vk_{some_name}_STAGETYPE, where _STAGETYPE suffix is
    // something from well defined set.
    const auto name = path.stem().string();

    // Shader file name must contain "vulkan shader stage attribute"
    // suffix - i.e. something from set "VERTEX", "FRAGMENT" etc.
    if ( !isValidName( name ) ) {
        throw std::runtime_error( std::format(
            "vk::ShaderStorage == shader file name \"{}\" not satisfies naming "
            "convention",
            name ) );
    }

    // Check if shader module with name exist in modules map
    if ( modules_.contains( name ) ) {
        log::warning(
            "vk::ShaderStorage == shader module with name \"{}\" exist, no "
            "need "
            "to "
            "replace it",
            name );
        return;
    }

    // Split given string by seperator
    auto split = []( const std::string &string, const char *sep ) {
        std::vector<std::string> list;
        std::string::size_type start{ 0 };
        std::string::size_type end;

        while ( ( end = string.find( sep, start ) ) != std::string::npos ) {
            if ( start != end )
                list.push_back( string.substr( start, end - start ) );
            start = end + 1;
        }

        if ( start != string.size() ) list.push_back( string.substr( start ) );

        return list;
    };

    // Check if same shader stage already occupied
    const auto suffix = split( name, "_" ).back();
    if ( checkStageExist( suffix ) ) {
        log::warning(
            "vk::ShaderStorage == shader module for stage \"{}\" exist, no "
            "need "
            "to "
            "replace it",
            suffix );
    }

    // Read SPIRV file from disk
    const long long fileSize = file.tellg();
    std::vector<char> charBuf( fileSize );
    file.seekg( 0 );
    file.read( charBuf.data(), static_cast<int>( fileSize ) );
    file.close();

    std::vector<uint8_t> uint8Buf( fileSize );
    std::transform( charBuf.begin(), charBuf.end(), uint8Buf.begin(),
                    []( char v ) { return static_cast<uint8_t>( v ); } );

    push( uint8Buf, name );
}

void ShaderStorage::add( std::span<uint8_t> bytecode,
                         const std::string &name ) {
    push( bytecode, name );
}

void ShaderStorage::fill( const std::vector<std::filesystem::path> &files ) {
    if ( files.size() < 2 ) {
        throw std::runtime_error( std::format(
            "vk::ShaderStorage == pipeline shader storage must "
            "contains at least vertex and fragment shader stages!" ) );
    }

    for ( const auto &item : files ) {
        add( item );
    }
}

void ShaderStorage::fill(
    const std::vector<std::pair<std::span<uint8_t>, std::string>> &sources ) {
    if ( sources.size() < 2 ) {
        throw std::runtime_error( std::format(
            "vk::ShaderStorage == pipeline shader storage must "
            "contains at least vertex and fragment shader stages!" ) );
    }

    for ( const auto &item : sources ) {
        auto [bytecode, name] = item;
        add( bytecode, name );
    }
}

VkShaderModule ShaderStorage::get( const std::string &name ) {
    VkShaderModule module;
    try {
        module = modules_.at( name );
    } catch ( std::out_of_range &e ) {
        log::warning( "module {} not exist!", name );
        return VK_NULL_HANDLE;
    }
    return module;
}

void ShaderStorage::destroy( const std::string &name ) {
    VkShaderModule module;
    try {
        module = modules_.at( name );
    } catch ( std::out_of_range &e ) {
        log::warning( "module {} not exist!", name );
        return;
    }
    vkDestroyShaderModule( device_->handle(), module, nullptr );
    modules_.erase( name );
}

void ShaderStorage::list() {
    for ( const auto &key : modules_ ) {
        log::debug<DEBUG_OUTPUT_SHADER_STORAGE_VK_CPP>(
            "available shader module: \"{}\"", std::get<0>( key ) );
    }
}

bool ShaderStorage::checkStageExist( const std::string &stageSuffix ) {
    // Find shader stage module name in modules_ which have certain suffix
    const auto end = modules_.cend();
    const auto it = std::find_if(
        modules_.cbegin(), end,
        [id = stageSuffix]( std::pair<std::string, VkShaderModule> item ) {
            return std::get<0>( item ).ends_with( id );
        } );

    return it != end;
}

bool ShaderStorage::isValidName( const std::string &name ) {
    // Finds out that given shader file name contains somthing from
    // shader stage suffix set ("VERTEX", "FRAGMENT" etc.)
    const auto end = StagesSuffixMap.cend();
    const auto it = std::find_if(
        StagesSuffixMap.cbegin(), end,
        [id = name]( std::pair<ShaderStageType, std::string> item ) {
            return id.ends_with( std::get<1>( item ) );
        } );

    return it != end;
}

}  // namespace tire::vk
