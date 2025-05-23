

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <format>
#include <print>
#include <map>
#include <coroutine>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "../render.h"
#include "render/gl/program.h"
#include "scene.h"
#include "screen_string.h"
#include "algebra/vector2.h"
#include "algebra/vector3.h"

namespace tire {

using namespace tire::gl;

struct Framebuffer final {
    GLuint frameTexture_{};
    GLuint frambufferObject_{};
    GLuint renderbufferObject_{};

    std::array<algebra::vector3f, 4> frameQuadCrds{
        algebra::vector3f{ -1.0f, 1.0f, 0.0f },
        algebra::vector3f{ 1.0f, 1.0f, 0.0f },
        algebra::vector3f{ 1.0f, -1.0f, 0.0f },
        algebra::vector3f{ -1.0f, -1.0f, 0.0f } };

    std::array<algebra::vector2f, 4> frameQuadTexc{
        algebra::vector2f{ 0.0f, 1.0f }, algebra::vector2f{ 1.0f, 1.0f },
        algebra::vector2f{ 1.0f, 0.0f }, algebra::vector2f{ 0.0f, 0.0f } };

    GLuint array_{};
    std::array<GLuint, 2> buffers_{};
};

struct RenderGL final : Render {
    RenderGL();
    ~RenderGL() override;

    void displayRenderInfo() override;

private:
    void checkGlxVersion();
    void initGlxExtensions();
    void configureGl();
    void setupDebugMessages();

    std::string getGLVendorString() {
        return { (const char *)glGetString( GL_VENDOR ) };
    }

    std::string getGLRendererString() {
        return { (const char *)glGetString( GL_RENDERER ) };
    }

    std::string getGLVersionString() {
        return { (const char *)glGetString( GL_VERSION ) };
    }

    std::string getGLSLVendorString() {
        return { (const char *)glGetString( GL_SHADING_LANGUAGE_VERSION ) };
    }

    void scene( const std::filesystem::path &path ) override;
    void preLoop() override;
    void preFrame() override;
    void frame() override;
    void postFrame() override;
    void swapBuffers() override;
    void postLoop() override;

    void setSwapInterval( int interval ) override;

private:
    // glx extensions section
    using glXCreateContextAttribsARBProc = GLXContext ( * )( Display *,
                                                             GLXFBConfig,
                                                             GLXContext, Bool,
                                                             const int * );
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB{ nullptr };

    using glXSwapIntervalEXTProc = void ( * )( Display *, GLXDrawable, int );
    glXSwapIntervalEXTProc glXSwapIntervalEXT{ nullptr };
    static constexpr int GLX_SWAP_INTERVAL{ 0x20F1 };
    static constexpr int GLX_MAX_SWAP_INTEVAL{ 0x20F2 };

    // X11 OpenGL context
    GLXContext glContext_{ nullptr };

    std::unique_ptr<ScreenString> screenString_;

    Framebuffer framebuffer_{};
    Program frameProgram_{};
};

}  // namespace tire
