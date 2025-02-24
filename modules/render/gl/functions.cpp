
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

#include "log/log.h"
#include "functions.h"

namespace tire::gl {

// shaders
PFNGLCREATEPROGRAMPROC glCreateProgram{ nullptr };
PFNGLLINKPROGRAMPROC glLinkProgram{ nullptr };
PFNGLUSEPROGRAMPROC glUseProgram{ nullptr };
PFNGLSHADERSOURCEPROC glShaderSource{ nullptr };
PFNGLCOMPILESHADERPROC glCompileShader{ nullptr };
PFNGLCREATESHADERPROC glCreateShader{ nullptr };
PFNGLGETSHADERIVPROC glGetShaderiv{ nullptr };
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog{ nullptr };
PFNGLDELETESHADERPROC glDeleteShader{ nullptr };
PFNGLDELETEPROGRAMPROC glDeleteProgram{ nullptr };
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform{ nullptr };
PFNGLGETPROGRAMIVPROC glGetProgramiv{ nullptr };
PFNGLATTACHSHADERPROC glAttachShader{ nullptr };
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog{ nullptr };
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation{ nullptr };
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib{ nullptr };

PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv{ nullptr };
PFNGLUNIFORM3FVPROC glUniform3fv{ nullptr };

// VAO
PFNGLGENBUFFERSPROC glGenBuffers{ nullptr };
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays{ nullptr };
PFNGLBINDBUFFERPROC glBindBuffer{ nullptr };
PFNGLBINDVERTEXARRAYPROC glBindVertexArray{ nullptr };
PFNGLBUFFERDATAPROC glBufferData{ nullptr };
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer{ nullptr };
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray{ nullptr };
PFNGLBUFFERSUBDATAPROC glBufferSubData{ nullptr };
PFNGLDRAWARRAYSEXTPROC glDrawArrays{ nullptr };
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray{ nullptr };
PFNGLDELETEBUFFERSPROC glDeleteBuffers{ nullptr };
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays{ nullptr };
PFNGLDRAWELEMENTSPROC glDrawElements{ nullptr };

PFNGLMAPBUFFERPROC glMapBuffer{ nullptr };
PFNGLUNMAPBUFFERPROC glUnmapBuffer{ nullptr };

PFNGLGENERATEMIPMAPPROC glGenerateMipmap{ nullptr };

void init() {
    // shaders
    glCreateProgram =
        reinterpret_cast<PFNGLCREATEPROGRAMPROC>( glXGetProcAddress(
            reinterpret_cast<const GLubyte *>( "glCreateProgram" ) ) );

    glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(
        glXGetProcAddress( (const GLubyte *)"glLinkProgram" ) );

    glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(
        glXGetProcAddress( (const GLubyte *)"glUseProgram" ) );

    glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(
        glXGetProcAddress( (const GLubyte *)"glShaderSource" ) );

    glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(
        glXGetProcAddress( (const GLubyte *)"glCompileShader" ) );

    glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(
        glXGetProcAddress( (const GLubyte *)"glCreateShader" ) );

    glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>(
        glXGetProcAddress( (const GLubyte *)"glGetShaderiv" ) );

    glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(
        glXGetProcAddress( (const GLubyte *)"glGetShaderInfoLog" ) );

    glDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>(
        glXGetProcAddress( (const GLubyte *)"glDeleteShader" ) );

    glDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(
        glXGetProcAddress( (const GLubyte *)"glDeleteProgram" ) );

    glGetActiveUniform = reinterpret_cast<PFNGLGETACTIVEUNIFORMPROC>(
        glXGetProcAddress( (const GLubyte *)"glGetActiveUniform" ) );

    glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(
        glXGetProcAddress( (const GLubyte *)"glGetProgramiv" ) );

    glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(
        glXGetProcAddress( (const GLubyte *)"glAttachShader" ) );

    glGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(
        glXGetProcAddress( (const GLubyte *)"glGetProgramInfoLog" ) );

    glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(
        glXGetProcAddress( (const GLubyte *)"glGetUniformLocation" ) );

    glGetActiveAttrib = reinterpret_cast<PFNGLGETACTIVEATTRIBPROC>(
        glXGetProcAddress( (const GLubyte *)"glGetActiveAttrib" ) );

    glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(
        glXGetProcAddress( (const GLubyte *)"glUniformMatrix4fv" ) );

    glUniform3fv = reinterpret_cast<PFNGLUNIFORM3FVPROC>(
        glXGetProcAddress( (const GLubyte *)"glUniform3fv" ) );

    // VAO
    glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>( glXGetProcAddress(
        reinterpret_cast<const GLubyte *>( "glGenBuffers" ) ) );

    glGenVertexArrays =
        reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>( glXGetProcAddress(
            reinterpret_cast<const GLubyte *>( "glGenVertexArrays" ) ) );

    glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>( glXGetProcAddress(
        reinterpret_cast<const GLubyte *>( "glBindBuffer" ) ) );

    glBindVertexArray =
        reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>( glXGetProcAddress(
            reinterpret_cast<const GLubyte *>( "glBindVertexArray" ) ) );

    glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>( glXGetProcAddress(
        reinterpret_cast<const GLubyte *>( "glBufferData" ) ) );

    glVertexAttribPointer =
        reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>( glXGetProcAddress(
            reinterpret_cast<const GLubyte *>( "glVertexAttribPointer" ) ) );

    glEnableVertexAttribArray =
        reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(
            glXGetProcAddress( reinterpret_cast<const GLubyte *>(
                "glEnableVertexAttribArray" ) ) );

    glBufferSubData =
        reinterpret_cast<PFNGLBUFFERSUBDATAPROC>( glXGetProcAddress(
            reinterpret_cast<const GLubyte *>( "glBufferSubData" ) ) );

    glDrawArrays = reinterpret_cast<PFNGLDRAWARRAYSEXTPROC>( glXGetProcAddress(
        reinterpret_cast<const GLubyte *>( "glDrawArrays" ) ) );

    glDisableVertexAttribArray =
        reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYPROC>(
            glXGetProcAddress( reinterpret_cast<const GLubyte *>(
                "glDisableVertexAttribArray" ) ) );

    glDeleteBuffers =
        reinterpret_cast<PFNGLDELETEBUFFERSPROC>( glXGetProcAddress(
            reinterpret_cast<const GLubyte *>( "glDeleteBuffers" ) ) );

    glDeleteVertexArrays =
        reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>( glXGetProcAddress(
            reinterpret_cast<const GLubyte *>( "glDeleteVertexArrays" ) ) );

    glDrawElements = reinterpret_cast<PFNGLDRAWELEMENTSPROC>( glXGetProcAddress(
        reinterpret_cast<const GLubyte *>( "glDrawElements" ) ) );
    if ( !glDrawElements )
        log::error( "can't get glDrawElements function address!" );

    glMapBuffer = reinterpret_cast<PFNGLMAPBUFFERPROC>( glXGetProcAddress(
        reinterpret_cast<const GLubyte *>( "glMapBuffer" ) ) );
    if ( !glMapBuffer ) log::error( "can't get glMapBuffer function address!" );

    glUnmapBuffer = reinterpret_cast<PFNGLUNMAPBUFFERPROC>( glXGetProcAddress(
        reinterpret_cast<const GLubyte *>( "glUnmapBuffer" ) ) );
    if ( !glUnmapBuffer )
        log::error( "can't get glUnmapBuffer function address!" );

    glGenerateMipmap =
        reinterpret_cast<PFNGLGENERATEMIPMAPPROC>( glXGetProcAddress(
            reinterpret_cast<const GLubyte *>( "glGenerateMipmap" ) ) );
    if ( !glGenerateMipmap )
        log::error( "can't get glGenerateMipmap function address!" );
}

}  // namespace tire::gl
