
#include <print>
#include <format>
#include <set>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "rendervk.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };
#include "algebra/matrix4.h"

import config;

namespace tire {

RenderVK::RenderVK()
    : Render{} {
    try {
        instance_ = std::make_unique<vk::Instance>();

        surface_ =
            std::make_unique<vk::Surface>( display_, window_, instance_.get() );

        device_ =
            std::make_unique<vk::Device>( instance_.get(), surface_.get() );
        device_->pickAndCreateDevice();

        const auto configHandle = Config::instance();
        const auto basePath = configHandle->getBasePath().string();

        commandPool_ = std::make_unique<vk::CommandPool>( device_.get() );

        swapchain_ = std::make_unique<vk::Swapchain>(
            device_.get(), surface_.get(), commandPool_.get() );
        swapchain_->createSwapchain();
        swapchain_->createImageViews();

        piplineMatrixReady_ =
            std::make_unique<vk::PiplineMatrixReady>( device_.get() );
        piplineMatrixReady_->initPipeline(
            { basePath + "/assets/shaders/vk_simple_box_VERTEX.spv",
              basePath + "/assets/shaders/vk_simple_box_FRAGMENT.spv" } );

        swapchain_->createFramebuffers( piplineMatrixReady_.get() );

        present_ =
            std::make_unique<vk::Present>( device_.get(), swapchain_.get() );

        presentSync_ = std::make_unique<
            vk::PresentSynchronization<FRAMES_IN_FLIGHT_COUNT>>(
            device_.get() );

    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

void RenderVK::scene( const std::filesystem::path &path ) {
    const auto configHandle = Config::instance();
    const auto basePath = configHandle->getBasePath().string();

    piplineVertexBuffer_ =
        std::make_unique<vk::PiplineVertexBuffer>( device_.get() );
    piplineVertexBuffer_->initPipeline(
        { basePath + "/assets/shaders/vk_vertex_buffer_VERTEX.spv",
          basePath + "/assets/shaders/vk_vertex_buffer_FRAGMENT.spv" } );

    scene_ = std::make_shared<vk::Scene>( path, device_.get(),
                                          piplineVertexBuffer_.get() );
}

void RenderVK::setSwapInterval( int interval ) {

};

}  // namespace tire
