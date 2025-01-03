
#pragma once

#include <limits>

#include "algebra/vector3.h"
#include "algebra/matrix3.h"
#include "algebra/matrix4.h"

namespace tire {

template <typename T>
struct normal {
    using scalar_type = T;
    using self = normal<scalar_type>;
    using vec_type = algebra::vector3<scalar_type>;
    using mat3_type = algebra::matrix3<scalar_type>;
    using mat4_type = algebra::matrix4<scalar_type>;

    [[nodiscard]] scalar_type x() const { return normal_.x(); }
    [[nodiscard]] scalar_type y() const { return normal_.y(); }
    [[nodiscard]] scalar_type z() const { return normal_.z(); }

    // scalar_type dot(vec_type rhs) {
    //     return normal_.dot(rhs);
    // }

    // sualar_type squareLength() {
    //     return normal_.dot(normal_);
    // }

    // scalar_type lenght() {
    //     return normal_.length();
    // }

    // void normalize() {
    //     normal_.normalize();
    // }

    [[nodiscard]] self transform( mat3_type mtrx ) const {
        auto normal = normal_;
        auto i = mtrx.inverse();
        auto ti = i.transpose();
        return mtrx.mult_vector3( normal );
    }

    void transformThis( mat3_type mtrx ) {
        auto i = mtrx.inverse();
        auto ti = i.transpose();
        normal_ = ti.mult_vector3( normal_ );
    }

    [[nodiscard]] self transform( mat4_type mtrx ) const {
        auto normal = normal_;
        auto i = mtrx.inverse();
        auto ti = i.transpose();
        return mtrx.mult_vector3( normal );
    }

    void transformThis( mat4_type mtrx ) {
        auto i = mtrx.inverse();
        auto ti = i.transpose();
        normal_ = ti.mult_vector3( normal_ );
    }

private:
    vec_type normal_;
};

using normall = normal<long long>;
using normalf = normal<float>;
using normald = normal<double>;

}  // namespace tire
