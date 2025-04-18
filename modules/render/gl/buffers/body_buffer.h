
#pragma once

#include <array>
#include <GL/gl.h>

#include "../texture.h"

namespace tire::gl {

#define OBJECT_DEFUALT 0

#define VERTEX_BUFFER 0
#define NORMAL_BUFFER 1
#define TEXCRD_BUFFER 2

struct BodyBuffer final {
    BodyBuffer() = default;

    BodyBuffer( const BodyBuffer &other ) = delete;

    BodyBuffer( BodyBuffer &&other ) noexcept {
        this->verteciesCount_ = other.verteciesCount_;
        this->array_ = other.array_;
        this->buffers_ = other.buffers_;

        other.verteciesCount_ = 0;
        other.array_ = 0;
        other.buffers_[VERTEX_BUFFER] = 0;
        other.buffers_[NORMAL_BUFFER] = 0;
        other.buffers_[TEXCRD_BUFFER] = 0;
    };

    BodyBuffer &operator=( const BodyBuffer &other ) = delete;

    BodyBuffer &operator=( BodyBuffer &&other ) noexcept {
        if ( &other != this ) {
            this->verteciesCount_ = other.verteciesCount_;
            this->array_ = other.array_;
            this->buffers_ = other.buffers_;

            other.verteciesCount_ = 0;
            other.array_ = 0;
            other.buffers_[VERTEX_BUFFER] = 0;
            other.buffers_[NORMAL_BUFFER] = 0;
            other.buffers_[TEXCRD_BUFFER] = 0;
        }

        return *this;
    };

    ~BodyBuffer() { clean(); };

    void generate();
    void clean();

    void bind();
    void bindVertexData( long verteciesCount, const void *data );
    void bindNormalData( long verteciesCount, const void *data );
    void bindTexcrdData( long verteciesCount, const void *data );

    void updateVertexData( long size, const void *data );
    void updateNormalsData( long size, const void *data );

    void release();

    void draw();

private:
    GLuint verteciesCount_{};

    // Body geometry information.
    // glDeleteBuffers() glDeleteVertexArrays() silently ignores 0's and names
    // that do not correspond to existing buffer objects.
    GLuint array_{ OBJECT_DEFUALT };
    std::array<GLuint, 3> buffers_{ OBJECT_DEFUALT };
};

}  // namespace tire::gl
