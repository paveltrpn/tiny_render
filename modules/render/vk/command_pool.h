 
#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "device.h"

namespace tire::vk {

struct CommandPool final {
    CommandPool( const vk::Device *device );

    CommandPool( const CommandPool &other ) = delete;
    CommandPool( CommandPool &&other ) = delete;
    CommandPool &operator=( const CommandPool &other ) = delete;
    CommandPool &operator=( CommandPool &&other ) = delete;

    ~CommandPool() {
        vkDestroyCommandPool( device_->handle(), commandPool_, nullptr );
    }

    [[nodiscard]] VkCommandPool handle() const { return commandPool_; }

private:
    const vk::Device *device_{};

    // Vulkan etity handles
    VkCommandPool commandPool_{ VK_NULL_HANDLE };

    // Vulkan related structures
    VkCommandPoolCreateInfo poolInfo_{};
};

}
