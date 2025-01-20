
#pragma once

#include <vector>
#include <optional>
#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"

namespace tire::vk {

struct Instance final {
    Instance();

    Instance( const Instance &other ) = delete;
    Instance( Instance &&other ) = delete;
    Instance &operator=( const Instance &other ) = delete;
    Instance &operator=( Instance &&other ) = delete;

    ~Instance();

    [[nodiscard]] VkInstance handle() const;
    [[nodiscard]] std::pair<uint32_t, char *const *const> validationLayersInfo()
        const;

    void info() const;

private:
    // pass std::nullopt to enable all available exensions
    std::vector<char *> makeExtensionsList(
        std::optional<std::vector<std::string>> list );

    // pass std::nullopt to enable all available validation layers.
    std::vector<char *> makeValidationLayersList(
        std::optional<std::vector<std::string>> list );

private:
    // Vulkan etity handles
    VkInstance instance_{ VK_NULL_HANDLE };
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };

    std::vector<char *> acquiredValidationLayers_{};

    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};
};

}  // namespace tire::vk
