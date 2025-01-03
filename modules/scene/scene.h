
#pragma once

#include <filesystem>
#include "nlohmann/json.hpp"

#include "node.h"
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

    void traverse();

private:
    void process();

private:
    static const Box box;
    nlohmann::json scene_;

protected:
    std::vector<std::shared_ptr<Node>> nodeList_;
    std::vector<std::shared_ptr<Camera>> cameras_{};
};

}  // namespace tire
