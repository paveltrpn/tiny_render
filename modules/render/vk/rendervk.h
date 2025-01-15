
#pragma once

#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <expected>
#include <variant>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "instance.h"
#include "surface.h"

#include "pipeline.h"
#include "commands.h"

#include "config/config.h"
#include "../render.h"
#include "shader_storage.h"
#include "scene.h"

namespace tire {

struct RenderVK final : Render {
    RenderVK();
    ~RenderVK() override;

    void displayRenderInfo() override;
    void setSwapInterval( int interval ) override;

private:
    void scene( const std::filesystem::path &path ) override;
    void preLoop() override;
    void preFrame() override;
    void frame() override;
    void postFrame() override;
    void swapBuffers() override;
    void postLoop() override;

    void initPhysicalDevices();
    void pickAndCreateDevice(
        size_t id );  // TODO: make physical device pick smarter
    void createSwapchain();
    void createImageViews();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createSyncObjects();

    void displayPhysicalDeviceProperties( size_t id );
    void displayPhysicalDeviceFeatures( size_t id );
    void displayPhysicalDeviceFamiliesProperties( size_t id );
    void displaySurfaceCapabilities();

private:
    std::shared_ptr<vk::Scene> scene_;

    std::unique_ptr<vk::Instance> instance_{};
    std::unique_ptr<vk::Surface> surface_{};

    std::unique_ptr<vk::ShaderStorage> shaderStorage_{};
    std::unique_ptr<vk::PiplineSimple> pipelineSimple_{};
    std::unique_ptr<CommandPool> commandPool_{};
    std::vector<std::unique_ptr<CommandBuffer>> cBufs_{};

    // handles
    VkDevice device_{ VK_NULL_HANDLE };
    VkQueue graphicsQueue_{ VK_NULL_HANDLE };
    VkQueue presentQueue_{ VK_NULL_HANDLE };
    VkSwapchainKHR swapChain_{ VK_NULL_HANDLE };
    std::vector<VkImage> swapChainImages_{};
    std::vector<VkImageView> swapChainImageViews_{};
    std::vector<VkFramebuffer> framebuffers_{};
    std::vector<VkSemaphore> imageAvailableSemaphores_{};
    std::vector<VkSemaphore> renderFinishedSemaphores_{};
    std::vector<VkFence> inFlightFences_{};

    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};
    std::vector<VkSurfaceFormatKHR> surfaceFormats_{};
    std::vector<VkPresentModeKHR> presentModes_{};

    struct PhysicalDevice {
        VkPhysicalDevice device{ VK_NULL_HANDLE };
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
    };
    uint32_t graphicsFamily_{};
    uint32_t presentFamily_{};

    std::vector<PhysicalDevice> physicalDevices_{};

    VkFormat swapChainImageFormat_{};
    VkExtent2D swapChainExtent_{};

    uint32_t currentFrame_{ 0 };
};

}  // namespace tire
