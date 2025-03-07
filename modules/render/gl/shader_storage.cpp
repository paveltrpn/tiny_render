
#include <fstream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "shader_storage.h"
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include "functions.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SHADER_STORAGE_GL_CPP{ true };

import config;

namespace tire::gl {

void ShaderStorage::add( const std::string &name ) {
    std::vector<GLuint> stageUnits{};

    // Try to find all files in assets directory, that satisfies
    // shader filename template, i.e. "gl_{name}_STAGETYPE.glsl"
    const auto &availableShaderFiles = scanForShaderFiles( name );

    // Iterate over available glsl shader files
    for ( const auto &[stage, type] : availableShaderFiles ) {
        // Shader source code as std::string
        const auto source = readSource( stage );
        // Compiled shader stage unit
        const auto unit = compile( type, source );
        // Store compiled shader unit
        stageUnits.push_back( unit );
    }

    // Link
    auto program = glCreateProgram();

    for ( const auto handle : stageUnits ) {
        glAttachShader( program, handle );
    }

    glLinkProgram( program );

    GLint success;
    glGetProgramiv( program, GL_LINK_STATUS, &success );

    if ( success == GL_FALSE ) {
        int32_t logLength;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
        std::vector<GLchar> log;
        log.reserve( logLength );
        glGetProgramInfoLog( program, logLength, nullptr, log.data() );
        throw std::runtime_error( std::format(
            "gl::ShaderStorage === can't link program with trace:\n{}",
            log.data() ) );
    }

    programs_[name] = program;
}

std::vector<std::pair<std::string, GLenum>> ShaderStorage::scanForShaderFiles(
    const std::string &name ) {
    /* 
     * @brief - Split given string by seperator
     * @param {std::string} - string to split
     * @param {const char *} - seperator
     * @retval {std::vector<std::string>} - list of partitions
     **/
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

    // Find out shader files directory location
    const auto configHandle = Config::instance();
    const auto basePath = configHandle->getBasePath().string();
    const std::filesystem::path shaderFilesPath =
        std::format( "{}/assets/shaders/", basePath );

    // Iterate over directory
    std::vector<std::pair<std::string, GLenum>> retItem;
    for ( auto const &entry :
          std::filesystem::directory_iterator{ shaderFilesPath } ) {
        // Take only "name" part of filename, i.e. except
        // extension and path.
        const auto &fileName = entry.path().stem().string();

        // Check if file contains shader file name template.
        // Name template is "gl_{name}_{type}".
        if ( fileName.contains( std::format( "gl_{}_", name ) ) ) {
            // Take suffix from name, i.e. _{type}
            const auto suffix = split( fileName, "_" ).back();

            // Obtain shader file stage type or throw error
            GLenum type;
            try {
                type = StagesSuffixMap.at( suffix );
            } catch ( std::out_of_range &e ) {
                throw std::runtime_error( std::format(
                    "gl::ShaderStorage === shader stage type {} not exist!",
                    suffix ) );
            }

            // Store available shader file name and it's shader stage type
            retItem.push_back(
                std::make_pair( std::format( "{}.glsl", fileName ), type ) );
        }
    }

    // If directory not contains shader source files then throw
    if ( retItem.empty() ) {
        throw std::runtime_error(
            std::format( "gl::ShaderStorage === shaders directory not contain "
                         "files for shader {}",
                         name ) );
    }

    return retItem;
}

std::string ShaderStorage::readSource( const std::string &name ) {
    const auto configHandle = Config::instance();
    const auto basePath = configHandle->getBasePath().string();
    const std::filesystem::path path =
        std::format( "{}/assets/shaders/{}", basePath, name );

    log::debug<DEBUG_OUTPUT_SHADER_STORAGE_GL_CPP>(
        "gl::ShaderStorage === loading shader file {}",
        path.filename().string() );

    std::ifstream file{ path };
    if ( !file ) {
        throw std::runtime_error(
            std::format( "gl::ShaderStorage === file not found: {}\n",
                         path.filename().string() ) );
    }

    // Read file into string
    std::string str;
    file.seekg( 0, std::ios::end );
    str.reserve( file.tellg() );
    file.seekg( 0, std::ios::beg );

    str.assign( ( std::istreambuf_iterator<char>( file ) ),
                std::istreambuf_iterator<char>() );
    file.close();

    return str;
}

GLuint ShaderStorage::compile( GLenum stage, std::string_view source ) {
    const GLuint shHandle = glCreateShader( stage );
    const char *c_str = source.data();

    glShaderSource( shHandle, 1, &c_str, nullptr );
    glCompileShader( shHandle );

    GLint success;

    glGetShaderiv( shHandle, GL_COMPILE_STATUS, &success );
    if ( success == GL_FALSE ) {
        int32_t logLength;
        glGetShaderiv( shHandle, GL_INFO_LOG_LENGTH, &logLength );
        std::vector<GLchar> log;
        log.reserve( logLength );
        glGetShaderInfoLog( shHandle, logLength, nullptr, log.data() );
        throw std::runtime_error( std::format(
            "gl::ShaderStorage === can't compile program with trace:\n{}",
            log.data() ) );
    } else {
        return shHandle;
    }
}

void ShaderStorage::use( const std::string &name ) {
    // GLuint module;
    // try {
    // module = programs_.at( name );
    // } catch ( std::out_of_range &e ) {
    // throw std::runtime_error( std::format(
    // "gl::ShaderStorage === shader program {} not exist!", name ) );
    // }

    // Just belive in
    glUseProgram( programs_[name] );
}

void ShaderStorage::destroy( const std::string &name ) {
    // for ( const auto handle : stages_ ) {
    // glDeleteProgram( handle );
    // }
}

GLuint ShaderStorage::getUniformLocation( const std::string &name,
                                          const char *id ) {
    const auto location = glGetUniformLocation( programs_[name], id );

    if ( location == GL_INVALID_VALUE ) {
        log::warning(
            "gl::ShaderStorage ===  uniform location error - invalid value" );
    }

    if ( location == GL_INVALID_OPERATION ) {
        log::warning(
            "gl::ShaderStorage ===  uniform location error - invalid "
            "operation" );
    }

    return location;
}
}  // namespace tire::gl
