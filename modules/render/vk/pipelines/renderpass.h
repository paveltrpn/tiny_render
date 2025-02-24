
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <print>

#include <log/log.h>

#include "../device.h"

namespace tire::vk {

struct RenderpassBase {
    RenderpassBase( const vk::Device *device )
        : device_{ device } {}

    RenderpassBase( const RenderpassBase &other ) = delete;
    RenderpassBase( RenderpassBase &&other ) = delete;

    RenderpassBase &operator=( const RenderpassBase &other ) = delete;
    RenderpassBase &operator=( RenderpassBase &&other ) = delete;

    virtual ~RenderpassBase() {
        vkDestroyRenderPass( device_->handle(), renderPass_, nullptr );
    };

    [[nodiscard]] VkRenderPass handle() const { return renderPass_; }

    virtual void init() = 0;

protected:
    const vk::Device *device_;
    VkRenderPass renderPass_{ VK_NULL_HANDLE };
};

// ==========================================================================

struct RenderpassSimple final : RenderpassBase {
    using base_class = RenderpassBase;

    RenderpassSimple( const vk::Device *device )
        : base_class{ device } {
        init();
    }

    void init() override;
};

}  // namespace tire::vk
