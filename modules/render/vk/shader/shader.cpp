
#include <fstream>
#include <vector>
#include <vulkan/vk_enum_string_helper.h>

#include "shader.h"
#include "log/log.h"

namespace tire::vk {

Shader::Shader( const VkDevice &device )
    : device_{ device } {
}

Shader::~Shader() {
    for ( const auto &module : modules_ ) {
        vkDestroyShaderModule( device_, std::get<1>( module ), nullptr );
    }
}

void Shader::add( const std::string &path, const std::string &name ) {
    std::ifstream file( path, std::ios::ate | std::ios::binary );
    if ( !file.is_open() ) {
        throw std::runtime_error(
            std::format( "failed to open file {}!", path ) );
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer( fileSize );
    file.seekg( 0 );
    file.read( buffer.data(), fileSize );
    file.close();

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>( buffer.data() );

    VkShaderModule module;
    const auto err =
        vkCreateShaderModule( device_, &createInfo, nullptr, &module );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create shader module {} with code {}!",
                         name, string_VkResult( err ) ) );
    } else {
        log::info( "shader module {} created!", name );
    }

    modules_[name] = module;
}

VkShaderModule Shader::get( const std::string &name ) {
    VkShaderModule module;
    try {
        module = modules_.at( name );
    } catch ( std::out_of_range &e ) {
        log::warning( "module {} not exist!", name );
        return VK_NULL_HANDLE;
    }
    return module;
}

}  // namespace tire::vk