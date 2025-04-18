#pragma once

#include <type_traits>

#include "algebra/vector3.h"

namespace tire {

using namespace algebra;

template <typename T>
    requires std::is_same_v<float, T> ||
    std::is_same_v<double, T> struct OmniLight final {
    using value_type = T;

    OmniLight() = default;

    OmniLight( const OmniLight& other ) = delete;
    OmniLight( OmniLight&& other ) = delete;
    OmniLight& operator=( const OmniLight& other ) = delete;
    OmniLight& operator=( OmniLight&& other ) = delete;

    ~OmniLight() = default;

    [[nodiscard]]
    vector3<value_type> position() {
        return position_;
    };

    [[nodiscard]]
    value_type constant() {
        return constant_;
    };

    [[nodiscard]]
    value_type linear() {
        return linear_;
    };

    [[nodiscard]]
    value_type quadratic() {
        return quadratic_;
    };

    [[nodiscard]]
    vector3<value_type> ambient() {
        return ambient_;
    };

    [[nodiscard]]
    vector3<value_type> diffuse() {
        return diffuse_;
    };

    [[nodiscard]]
    vector3<value_type> specular() {
        return specular_;
    };

    void setPosition( const vector3<value_type>& value ) { position_ = value; };

    void setConstant( value_type value ) { constant_ = value; };
    void setLinear( value_type value ) { linear_ = value; };
    void setQuadratic( value_type value ) { quadratic_ = value; };

    void setAmbient( const vector3<value_type>& value ) { ambient_ = value; };
    void setDiffuse( const vector3<value_type>& value ) { diffuse_ = value; };
    void setSpecular( const vector3<value_type>& value ) { specular_ = value; };

private:
    vector3<value_type> position_{ 0.0, 0.0, 0.0 };

    value_type constant_{ 1.0 };
    value_type linear_{ 1.0 };
    value_type quadratic_{ 1.0 };

    vector3<value_type> ambient_{ 1.0, 1.0, 1.0 };
    vector3<value_type> diffuse_{ 1.0, 1.0, 1.0 };
    vector3<value_type> specular_{ 1.0, 1.0, 1.0 };
};

}  // namespace tire
