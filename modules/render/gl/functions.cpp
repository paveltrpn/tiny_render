
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>
#include <stdexcept>

#include "log/log.h"
#include "functions.h"

namespace tire::gl {

PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback{ nullptr };

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

PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv{ nullptr };
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv{ nullptr };
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv{ nullptr };
PFNGLUNIFORM2FVPROC glUniform2fv{ nullptr };
PFNGLUNIFORM3FVPROC glUniform3fv{ nullptr };
PFNGLUNIFORM4FVPROC glUniform4fv{ nullptr };
PFNGLUNIFORM1FPROC glUniform1f{ nullptr };
PFNGLUNIFORM1IPROC glUniform1i{ nullptr };
PFNGLUNIFORM1UIPROC glUniform1ui{ nullptr };

// GL_ARB_gpu_shader_fp64
PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv{ nullptr };
PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv{ nullptr };
PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv{ nullptr };
PFNGLUNIFORM2DVPROC glUniform2dv{ nullptr };
PFNGLUNIFORM3DVPROC glUniform3dv{ nullptr };
PFNGLUNIFORM4DVPROC glUniform4dv{ nullptr };
PFNGLUNIFORM1DPROC glUniform1d{ nullptr };

// Vertex buffers
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

// Textures
PFNGLGENERATEMIPMAPPROC glGenerateMipmap{ nullptr };
PFNGLCREATETEXTURESPROC glCreateTextures{ nullptr };
PFNGLTEXTURESTORAGE1DPROC glTextureStorage1D{ nullptr };
PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D{ nullptr };
PFNGLTEXTURESTORAGE3DPROC glTextureStorage3D{ nullptr };
PFNGLTEXTURESUBIMAGE1DPROC glTextureSubImage1D{ nullptr };
PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D{ nullptr };
PFNGLTEXTURESUBIMAGE3DPROC glTextureSubImage3D{ nullptr };
PFNGLBINDTEXTUREUNITPROC glBindTextureUnit{ nullptr };
PFNGLCREATESAMPLERSPROC glCreateSamplers{ nullptr };
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri{ nullptr };
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf{ nullptr };
PFNGLBINDSAMPLERPROC glBindSampler{ nullptr };
PFNGLTEXTUREPARAMETERIPROC glTextureParameteri{ nullptr };

// Framebuffer / Renderbuffer
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers{ nullptr };
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer{ nullptr };
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus{ nullptr };
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers{ nullptr };
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D{ nullptr };

PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers{ nullptr };
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer{ nullptr };
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage{ nullptr };
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer{ nullptr };
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers{ nullptr };

namespace {

bool queryExtension( const char *extName ) {
    const auto extensionsCharPtr = glGetString( GL_EXTENSIONS );

    if ( extensionsCharPtr == nullptr ) {
        throw std::runtime_error( "can't obtain OpenGL extensions string!" );
    }

    const std::string extensionsString{
        reinterpret_cast<const char *>( extensionsCharPtr ) };

    if ( extensionsString.contains( extName ) ) {
        return true;
    }

    return false;
}

template <typename FuncPtr>
FuncPtr getProcAddress( const char *name ) {
    // A non-NULL return value for glXGetProcAddress does not guarantee that
    // an extension function is actually supported at runtime. The client must
    // also query glGetString(GL_EXTENSIONS) or glXQueryExtensionsString
    // to determine if an extension is supported by a particular context.
    const auto ptr =
        glXGetProcAddress( reinterpret_cast<const GLubyte *>( name ) );

    return reinterpret_cast<FuncPtr>( ptr );
}

}  // namespace

void init() {
    glDebugMessageCallback = getProcAddress<PFNGLDEBUGMESSAGECALLBACKPROC>(
        "glDebugMessageCallback" );

    // Shaders
    glCreateProgram =
        getProcAddress<PFNGLCREATEPROGRAMPROC>( "glCreateProgram" );

    glLinkProgram = getProcAddress<PFNGLLINKPROGRAMPROC>( "glLinkProgram" );

    glUseProgram = getProcAddress<PFNGLUSEPROGRAMPROC>( "glUseProgram" );

    glShaderSource = getProcAddress<PFNGLSHADERSOURCEPROC>( "glShaderSource" );

    glCompileShader =
        getProcAddress<PFNGLCOMPILESHADERPROC>( "glCompileShader" );

    glCreateShader = getProcAddress<PFNGLCREATESHADERPROC>( "glCreateShader" );

    glGetShaderiv = getProcAddress<PFNGLGETSHADERIVPROC>( "glGetShaderiv" );

    glGetShaderInfoLog =
        getProcAddress<PFNGLGETSHADERINFOLOGPROC>( "glGetShaderInfoLog" );

    glDeleteShader = getProcAddress<PFNGLDELETESHADERPROC>( "glDeleteShader" );

    glDeleteProgram =
        getProcAddress<PFNGLDELETEPROGRAMPROC>( "glDeleteProgram" );

    glGetActiveUniform =
        getProcAddress<PFNGLGETACTIVEUNIFORMPROC>( "glGetActiveUniform" );

    glGetProgramiv = getProcAddress<PFNGLGETPROGRAMIVPROC>( "glGetProgramiv" );

    glAttachShader = getProcAddress<PFNGLATTACHSHADERPROC>( "glAttachShader" );

    glGetProgramInfoLog =
        getProcAddress<PFNGLGETPROGRAMINFOLOGPROC>( "glGetProgramInfoLog" );

    glGetUniformLocation =
        getProcAddress<PFNGLGETUNIFORMLOCATIONPROC>( "glGetUniformLocation" );

    glGetActiveAttrib =
        getProcAddress<PFNGLGETACTIVEATTRIBPROC>( "glGetActiveAttrib" );

    glUniformMatrix2fv =
        getProcAddress<PFNGLUNIFORMMATRIX2FVPROC>( "glUniformMatrix2fv" );

    glUniformMatrix3fv =
        getProcAddress<PFNGLUNIFORMMATRIX3FVPROC>( "glUniformMatrix3fv" );

    glUniformMatrix4fv =
        getProcAddress<PFNGLUNIFORMMATRIX4FVPROC>( "glUniformMatrix4fv" );

    glUniform2fv = getProcAddress<PFNGLUNIFORM2FVPROC>( "glUnifor23fv" );

    glUniform3fv = getProcAddress<PFNGLUNIFORM3FVPROC>( "glUniform3fv" );

    glUniform4fv = getProcAddress<PFNGLUNIFORM4FVPROC>( "glUniform4fv" );

    glUniform1f = getProcAddress<PFNGLUNIFORM1FPROC>( "glUniform1f" );

    glUniform1i = getProcAddress<PFNGLUNIFORM1IPROC>( "glUniform1i" );

    glUniform1ui = getProcAddress<PFNGLUNIFORM1UIPROC>( "glUniform1ui" );

    // GL_ARB_gpu_shader_fp64

    glUniformMatrix2dv =
        getProcAddress<PFNGLUNIFORMMATRIX2DVPROC>( "glUniformMatrix2dv" );

    glUniformMatrix3dv =
        getProcAddress<PFNGLUNIFORMMATRIX3DVPROC>( "glUniformMatrix3dv" );

    glUniformMatrix4dv =
        getProcAddress<PFNGLUNIFORMMATRIX4DVPROC>( "glUniformMatrix4dv" );

    glUniform2dv = getProcAddress<PFNGLUNIFORM2DVPROC>( "glUniform2dv" );

    glUniform3dv = getProcAddress<PFNGLUNIFORM3DVPROC>( "glUniform3dv" );

    glUniform4dv = getProcAddress<PFNGLUNIFORM4DVPROC>( "glUniform4dv" );

    glUniform1d = getProcAddress<PFNGLUNIFORM1DPROC>( "glUniform1d" );

    // Vertex buffers
    glGenBuffers = getProcAddress<PFNGLGENBUFFERSPROC>( "glGenBuffers" );

    glGenVertexArrays =
        getProcAddress<PFNGLGENVERTEXARRAYSPROC>( "glGenVertexArrays" );

    glBindBuffer = getProcAddress<PFNGLBINDBUFFERPROC>( "glBindBuffer" );

    glBindVertexArray =
        getProcAddress<PFNGLBINDVERTEXARRAYPROC>( "glBindVertexArray" );

    glBufferData = getProcAddress<PFNGLBUFFERDATAPROC>( "glBufferData" );

    glVertexAttribPointer =
        getProcAddress<PFNGLVERTEXATTRIBPOINTERPROC>( "glVertexAttribPointer" );

    glEnableVertexAttribArray =
        getProcAddress<PFNGLENABLEVERTEXATTRIBARRAYPROC>(
            "glEnableVertexAttribArray" );

    glBufferSubData =
        getProcAddress<PFNGLBUFFERSUBDATAPROC>( "glBufferSubData" );

    glDrawArrays = getProcAddress<PFNGLDRAWARRAYSEXTPROC>( "glDrawArrays" );

    glDisableVertexAttribArray =
        getProcAddress<PFNGLDISABLEVERTEXATTRIBARRAYPROC>(
            "glDisableVertexAttribArray" );

    glDeleteBuffers =
        getProcAddress<PFNGLDELETEBUFFERSPROC>( "glDeleteBuffers" );

    glDeleteVertexArrays =
        getProcAddress<PFNGLDELETEVERTEXARRAYSPROC>( "glDeleteVertexArrays" );

    glDrawElements = getProcAddress<PFNGLDRAWELEMENTSPROC>( "glDrawElements" );

    glMapBuffer = getProcAddress<PFNGLMAPBUFFERPROC>( "glMapBuffer" );

    glUnmapBuffer = getProcAddress<PFNGLUNMAPBUFFERPROC>( "glUnmapBuffer" );

    // Textures
    glGenerateMipmap =
        getProcAddress<PFNGLGENERATEMIPMAPPROC>( "glGenerateMipmap" );

    glCreateTextures =
        getProcAddress<PFNGLCREATETEXTURESPROC>( "glCreateTextures" );

    glTextureStorage1D =
        getProcAddress<PFNGLTEXTURESTORAGE1DPROC>( "glTextureStorage1D" );

    glTextureStorage2D =
        getProcAddress<PFNGLTEXTURESTORAGE2DPROC>( "glTextureStorage2D" );

    glTextureStorage3D =
        getProcAddress<PFNGLTEXTURESTORAGE3DPROC>( "glTextureStorage3D" );

    glTextureSubImage1D =
        getProcAddress<PFNGLTEXTURESUBIMAGE1DPROC>( "glTextureSubImage1D" );

    glTextureSubImage2D =
        getProcAddress<PFNGLTEXTURESUBIMAGE2DPROC>( "glTextureSubImage2D" );

    glTextureSubImage3D =
        getProcAddress<PFNGLTEXTURESUBIMAGE3DPROC>( "glTextureSubImage3D" );

    glBindTextureUnit =
        getProcAddress<PFNGLBINDTEXTUREUNITPROC>( "glBindTextureUnit" );

    glCreateSamplers =
        getProcAddress<PFNGLCREATESAMPLERSPROC>( "glCreateSamplers" );

    glSamplerParameteri =
        getProcAddress<PFNGLSAMPLERPARAMETERIPROC>( "glSamplerParameteri" );

    glSamplerParameterf =
        getProcAddress<PFNGLSAMPLERPARAMETERFPROC>( "glSamplerParameterf" );

    glTextureParameteri =
        getProcAddress<PFNGLTEXTUREPARAMETERIPROC>( "glTextureParameteri" );

    // Framebuffer / Renderbuffer
    glGenFramebuffers =
        getProcAddress<PFNGLGENFRAMEBUFFERSPROC>( "glGenFramebuffers" );

    glBindFramebuffer =
        getProcAddress<PFNGLBINDFRAMEBUFFERPROC>( "glBindFramebuffer" );

    glCheckFramebufferStatus = getProcAddress<PFNGLCHECKFRAMEBUFFERSTATUSPROC>(
        "glCheckFramebufferStatus" );

    glDeleteFramebuffers =
        getProcAddress<PFNGLDELETEFRAMEBUFFERSPROC>( "glDeleteFramebuffers" );

    glFramebufferTexture2D = getProcAddress<PFNGLFRAMEBUFFERTEXTURE2DPROC>(
        "glFramebufferTexture2D" );

    glGenRenderbuffers =
        getProcAddress<PFNGLGENRENDERBUFFERSPROC>( "glGenRenderbuffers" );

    glBindRenderbuffer =
        getProcAddress<PFNGLBINDRENDERBUFFERPROC>( "glBindRenderbuffer" );

    glRenderbufferStorage =
        getProcAddress<PFNGLRENDERBUFFERSTORAGEPROC>( "glRenderbufferStorage" );

    glFramebufferRenderbuffer =
        getProcAddress<PFNGLFRAMEBUFFERRENDERBUFFERPROC>(
            "glFramebufferRenderbuffer" );

    glDeleteRenderbuffers =
        getProcAddress<PFNGLDELETERENDERBUFFERSPROC>( "glDeleteRenderbuffers" );
}

}  // namespace tire::gl
