
#ifndef __rendergl_h__
#define __rendergl_h__

#include <string>
#include <iostream>
#include <format>
#include <print>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "config/Config.h"
#include "Render.h"
#include "GLFunctions.h"

namespace tire {

struct RenderGL final : Render {
        RenderGL(const tire::Config& config);
        ~RenderGL() override;

        void displayRenderInfo() override;

        void appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) override;

    private:
        void configureGl();
        void setupDebugMessages();

        void preFrame() override;
        void postFrame() override;
        void swapBuffers() override;

        void traverse() override;

        std::vector<GLuint> appendShader(std::vector<std::tuple<GLuint, std::string>> shaders);
        void linkProgram();

    private:
        GLXContext glContext_{ nullptr };

        // OpeneGL function pointers handler object
        std::shared_ptr<GLFunctions> gl;

        std::string vendor_{};
        std::string renderer_{};
        std::string glVersion_{};
        std::string glslVersion_{};

        GLuint bufferObject_{};
        GLuint vertexObject_{};
        GLuint programObject_{};
};

}  // namespace tire

#endif
