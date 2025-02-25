
#include "rendergl.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_MAIN_LOOP_GL_CPP{ true };

namespace tire {

void RenderGL::preLoop() {
    // Start metrics logging coroutine
    showMetrics();

    const auto backgroundColor = Colorf( "#0f0f0f" );
    glClearColor( backgroundColor.r(), backgroundColor.g(), backgroundColor.b(),
                  1.0f );
};

void RenderGL::preFrame() {
    // Update global timer
    timer_.update();
    // Update scene objects
    scene_->traverse( timer_.floatFrameDuration() );
}

void RenderGL::frame() {
    glViewport( 0, 0, width_, height_ );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    glDisable( GL_TEXTURE_2D );
    scene_->submit();
    scene_->draw();

    glDisable( GL_DEPTH_TEST );

    screenString_->set_text_position( -2.0f, 0.0f );
    screenString_->draw( std::format( "{}", timer_.averageFrameDuration() ) );
    screenString_->flush( scene_->getCamera( 0 )->matrix() );
}

void RenderGL::postFrame() {
}

void RenderGL::swapBuffers() {
    glXSwapBuffers( display_, window_ );
}

void RenderGL::postLoop() {
}

}  // namespace tire
