
#include <iostream>
#include <string>
#include "algebra/matrix4.h"
#include "algebra/vector2.h"

#include "screen_string.h"
#include <GL/gl.h>
#include <GL/glcorearb.h>

namespace tire::gl {

ScreenString::ScreenString( const std::string& name )
    : ScreenStringBase( name ) {
    // Prepare vertex buffer
    buffer_.generate();
    buffer_.startBinding();
    buffer_.bindVertexData( letterQuadsVertecies_.size(),
                            letterQuadsVertecies_.data() );
    buffer_.bindTexcrdData( letterQuadsTexcrds_.size(),
                            letterQuadsTexcrds_.data() );

    // Prepare texture
    texture_.generate();
    texture_.bind();
    texture_.load( fontImage_->widht(), fontImage_->height(),
                   fontImage_->data() );

    // Prepare shader storage
    shaderStorage_.add( "screen_string" );
};

void ScreenString::flush() {
    shaderStorage_.use( "screen_string" );

    // Upload data
    buffer_.updateVertexData( letterQuadsVertecies_.size(),
                              letterQuadsVertecies_.data() );
    buffer_.updateTexcrdData( letterQuadsTexcrds_.size(),
                              letterQuadsTexcrds_.data() );

    // After uploading vertex data to GPU
    // set CPU side buffers to 0.
    // May be use memset() instead? Ah, nah...
    std::fill( std::begin( letterQuadsVertecies_ ),
               std::end( letterQuadsVertecies_ ),
               algebra::vector2f{ 0.0f, 0.0f } );

    std::fill( std::begin( letterQuadsTexcrds_ ),
               std::end( letterQuadsTexcrds_ ),
               algebra::vector2f{ 0.0f, 0.0f } );

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR );
    texture_.bind();
    buffer_.draw();
    glDisable( GL_BLEND );

    bufferPos_ = 0;
}

}  // namespace tire::gl
