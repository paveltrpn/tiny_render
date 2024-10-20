
#include <memory>
#include <filesystem>

#include "spdlog/spdlog.h"

#include "subject.h"

import render;
import config;
import camera;
import toy_std;

int main(int argc, char **argv) {
    try {
        new tire::Config{ std::filesystem::path{
          "/mnt/main_disk/code/tiny_render/test/window/config.json" } };
    } catch (const std::exception &e) {
        spdlog::critical("caught exception: {}", e.what());
        return 0;
    }

    std::unique_ptr<tire::Render> rndr;
    try {
        rndr = std::make_unique<tire::RenderGL>();
    } catch (const std::exception &e) {
        spdlog::critical("caught exception: {}", e.what());
        return 0;
    }

    rndr->displayRenderInfo();

    initSubject(rndr.get());

    {
        auto configPtr = tire::Config::instance();
        auto width = static_cast<float>(configPtr->get<int>("window_width"));
        auto height = static_cast<float>(configPtr->get<int>("window_height"));
        auto camera
          = std::make_shared<tire::camera::Perspective>(50.0f, width / height, 0.1f, 100.0f);

        camera->move(toy::algebra::vector3<float>{ 8.0f, 0.0f, -20.0f });
        camera->rotate(0.0f, 20.0f, 0.0f);

        rndr->addCamera(camera);
    }

    rndr->run();

    return 0;
}
