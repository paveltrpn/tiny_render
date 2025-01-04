
#pragma once

#include <filesystem>
#include <GL/gl.h>

#include "scene/scene.h"
#include "shader_storage.h"
#include "program.h"

namespace tire::gl {

struct SceneNodeBufferObjects final {
    GLuint vertexArray{};
    GLuint vertexBuffer{};
    GLuint normalBuffer{};
    GLuint texcrdBuffer{};
};

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void initPrograms();

    void submit();
    void output();

private:
    std::vector<SceneNodeBufferObjects> buffersList_;

    gl::Program<ProgramColorTag> colorProgram_{};
    gl::Program<ProgramFlatShadeTag> flatshadeProgram_{};
    gl::Program<ProgramTextureTag> textureProgram_{};

    gl::ShaderStorage shaderStorage_{};
};

}  // namespace tire::gl
