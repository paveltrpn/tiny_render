
#include "scene.h"
#include "functions.h"

import config;

namespace tire::gl {

Scene::Scene( const std::filesystem::path &fname )
    : tire::Scene{ fname } {
    const auto nodeListSize = bodyList_.size();
    buffersList_.reserve( nodeListSize );

    for ( size_t i{}; i < nodeListSize; ++i ) {
        BodyBuffer buffer{};

        buffer.generate();
        buffer.startBinding();
        buffer.bindVertexData( bodyList_[i]->verteciesArraySize(),
                               bodyList_[i]->verteciesData() );
        buffer.bindNormalData( bodyList_[i]->normalsArraySize(),
                               bodyList_[i]->normalsData() );
        buffer.bindTexcrdData( bodyList_[i]->texcrdsArraySize(),
                               bodyList_[i]->texcrdsData() );

        // Create Texture object
        // glGenTextures( 1, &buf.texture );
        // glBindTexture( GL_TEXTURE_2D, buf.texture );
        //
        // const auto [width, height] = nodeList_[i]->textureSize();
        // const auto data = nodeList_[i]->textureData();
        // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        //  GL_LINEAR_MIPMAP_LINEAR );
        // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        //   GL_UNSIGNED_BYTE, data );
        // glGenerateMipmap( GL_TEXTURE_2D );

        // Store object buffers id's
        buffersList_.push_back( std::move( buffer ) );
    }

    // Load shaders
    shaderStorage_.add( "flatshade" );
    shaderStorage_.use( "flatshade" );
    const Colorf lightColor = { 1.0f, 1.0f, 1.0f };
    shaderStorage_.setVectorUniform(
        "flatshade", "lightcolor",
        algebra::vector3f{ lightColor.r(), lightColor.g(), lightColor.b() } );

    shaderStorage_.setVectorUniform( "flatshade", "lightpos",
                                     algebra::vector3f{ 10.0f, 0.0f, 5.0f } );

    // ========================================================================

    shaderStorage_.add( "diffuse_texture" );
}

void Scene::submit() {
    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        buffer.updateVertexData( bodyList_[i]->verteciesArraySize(),
                                 bodyList_[i]->verteciesData() );
        buffer.updateNormalsData( bodyList_[i]->normalsArraySize(),
                                  bodyList_[i]->normalsData() );
        ++i;
    }
}

void Scene::draw() {
    shaderStorage_.use( "flatshade" );
    shaderStorage_.setMatrixUniform( "flatshade", "view_matrix", GL_FALSE,
                                     getCamera( 0 )->matrix() );

    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        const Colorf bodyColor = bodyList_[i]->color();
        shaderStorage_.setVectorUniform(
            "flatshade", "color",
            algebra::vector3f{ bodyColor.r(), bodyColor.g(), bodyColor.b() } );

        // glBindTexture( GL_TEXTURE_2D, buffer.texture );
        buffer.draw();

        ++i;
    }
}

}  // namespace tire::gl
