
#pragma once

#include <filesystem>
#include "nlohmann/json.hpp"
#include "uv.h"

#include "body.h"
#include "camera.h"

namespace tire {

struct Scene {
    Scene( const std::filesystem::path &fname );

    Scene( const Scene &other ) = delete;
    Scene( Scene &&other ) = delete;
    Scene &operator=( const Scene &other ) = delete;
    Scene &operator=( Scene &&other ) = delete;

    virtual ~Scene() = default;

    [[nodiscard]] std::shared_ptr<Camera> getCamera( size_t id ) const {
        return cameras_[id];
    };

    void traverse( float frameDuration );

private:
    void process();

private:
    nlohmann::json scene_;

protected:
    float frameDuration_{};

    std::vector<std::shared_ptr<Body>> bodyList_{};
    std::vector<std::shared_ptr<Camera>> cameras_{};
};

}  // namespace tire
