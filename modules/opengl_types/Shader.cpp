
#include "Shader.h"
#include "spdlog/spdlog.h"

namespace tire::opengl {

void Shader::link(std::vector<std::pair<GLuint, std::string>> shaders) {
    auto shaderList = getShadersList(shaders);

    program_ = glCreateProgram();

    for (const auto &shHandle : shaderList) {
        glAttachShader(program_, shHandle);
    }

    glLinkProgram(program_);

    GLint success;
    glGetProgramiv(program_, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        int32_t logLength;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> log;
        log.reserve(logLength);

        glGetProgramInfoLog(program_, logLength, nullptr, log.data());

        spdlog::error("can't link program with trace:\n{}", log.data());
        return;
    }
}

void Shader::use() {
    glUseProgram(program_);
}

std::vector<GLuint> Shader::getShadersList(std::vector<std::pair<GLuint, std::string>> shaders) {
    std::vector<GLuint> rt;
    if (shaders.size() < 2) {
        spdlog::error(" wrong shader count!");
        return rt;
    } else {
        for (const auto &[type, source] : shaders) {
            GLuint shHandle = glCreateShader(type);

            const char *c_str = source.c_str();

            glShaderSource(shHandle, 1, &c_str, nullptr);
            glCompileShader(shHandle);

            GLint success;
            glGetShaderiv(shHandle, GL_COMPILE_STATUS, &success);

            if (success == GL_FALSE) {
                int32_t logLength;
                glGetShaderiv(shHandle, GL_INFO_LOG_LENGTH, &logLength);

                std::vector<GLchar> log;
                log.reserve(logLength);
                glGetShaderInfoLog(shHandle, logLength, nullptr, log.data());

                spdlog::error("can't compile shader with trace:\n{}", log.data());
                return rt;
            }

            rt.push_back(shHandle);
        }
        return rt;
    }
}

void Shader::getActiveAttributes() {
    GLint size;   // size of the variable
    GLenum type;  // type of the variable (float, vec3 or mat4, etc)

    constexpr GLsizei bufSize = 16;    // maximum name length
    std::array<GLchar, bufSize> name;  // variable name in GLSL
    GLsizei length;                    // name length

    GLint count;
    glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &count);

    for (GLint i = 0; i < count; i++) {
        glGetActiveAttrib(program_, (GLuint)i, bufSize, &length, &size, &type, name.data());
        attributes_.push_back({ name.data(), type });
    }
}

void Shader::getActiveUniforms() {
    GLint size;   // size of the variable
    GLenum type;  // type of the variable (float, vec3 or mat4, etc)

    constexpr GLsizei bufSize = 16;    // maximum name length
    std::array<GLchar, bufSize> name;  // variable name in GLSL
    GLsizei length;                    // name length

    GLint count;
    glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &count);

    for (GLint i = 0; i < count; i++) {
        glGetActiveUniform(program_, (GLuint)i, bufSize, &length, &size, &type, name.data());
        uniforms_.push_back({ name.data(), type });
    }
}

void Shader::showActiveUniforms() {
    for (const auto &[uniName, uniType] : uniforms_) {
        std::print("Uniform name - {}, type - {}\n", uniName, uniType);
    }
}

void Shader::showActiveAttributes() {
    for (const auto &[attrName, attrType] : attributes_) {
        std::print("Attribute name - {}, type - {}\n", attrName, attrType);
    }
}

GLuint Shader::getUniformLocation(const std::string &id) {
    return glGetUniformLocation(program_, id.c_str());
}

void Shader::reset() {
    glDeleteProgram(program_);
}

}  // namespace tire::opengl
