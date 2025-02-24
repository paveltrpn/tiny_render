
#include <array>

#include <vulkan/vk_enum_string_helper.h>

#include "rendervk.h"
#include "swapchain.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SWAPCHAINVK_CPP{ true };

import config;

namespace tire::vk {

Swapchain::Swapchain( const vk::Device *device, const vk::Surface *surface )
    : device_{ device }
    , surface_{ surface } {
}

Swapchain::~Swapchain() {
    for ( auto framebuffer : framebuffers_ ) {
        vkDestroyFramebuffer( device_->handle(), framebuffer, nullptr );
    }
    vkDestroySwapchainKHR( device_->handle(), swapchain_, nullptr );
}

void Swapchain::createSwapchain() {
    swapChainExtent_ = device_->extent();

    const auto surfaceFormat = device_->surfaceFormat();
    const auto presentMode = device_->presentMode();

    const auto surfaceCapabilities = device_->surfaceCapabilities();

    // Choose count of images to draw in.
    uint32_t imageCount{};
    {
        // However, simply sticking to this minimum means that we may sometimes have
        // to wait on the driver to complete internal operations before we can acquire
        // another image to render to. Therefore it is recommended to request at least one
        // more image than the minimum:
        imageCount = surfaceCapabilities.minImageCount + 1;

        // We should also make sure to not exceed the maximum number of images while
        // doing this, where 0 is a special value that means that there is no maximum:
        if ( surfaceCapabilities.maxImageCount > 0 &&
             imageCount > surfaceCapabilities.maxImageCount ) {
            imageCount = surfaceCapabilities.maxImageCount;
        }

        // Skip all logic above, just use to two images
        imageCount = FRAMES_IN_FLIGHT_COUNT;
    }

    log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
        "vk::Swapchain: vulkan swapchain surface capabilities image count: {}, "
        "with max is {}",
        imageCount, surfaceCapabilities.maxImageCount );

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface_->handle();

    // The implementation will either create the swapchain with at least
    // that many images, or it will fail to create the swapchain.
    createInfo.minImageCount = imageCount;

    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = swapChainExtent_;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    const uint32_t graphicsFamily{ device_->graphicsFamily() };
    const uint32_t presentFamily{ device_->presentFamily() };
    std::array<uint32_t, 2> queueFamilyIndices = { graphicsFamily,
                                                   presentFamily };
    if ( graphicsFamily != presentFamily ) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;      // Optional
        createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if ( const auto err = vkCreateSwapchainKHR( device_->handle(), &createInfo,
                                                nullptr, &swapchain_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error( std::format(
            "failed to create swapchain code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info( "vk::Swapchain ===  vulkan swapchain created!" );
    }

    // Get swapchain images count. Sudden, this number is
    // equal to previously defined in VkSwapchainCreateInfoKHR.minImageCount.
    // But we still try to get image count that way.
    if ( const auto err = vkGetSwapchainImagesKHR(
             device_->handle(), swapchain_, &swapchainImageCount_, nullptr );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to get swapchain images count with code {}\n!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
            "vk::Swapchain === swapchain images count: {}",
            swapchainImageCount_ );
    }

    swapChainImages_.resize( imageCount );

    if ( const auto err =
             vkGetSwapchainImagesKHR( device_->handle(), swapchain_,
                                      &imageCount, swapChainImages_.data() );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to get swapchain images with code {}\n!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
            "vk::Swapchain === images acquired!" );
    }

    swapChainImageFormat_ = surfaceFormat.format;
}

void Swapchain::createImageViews() {
    swapChainImageViews_.resize( swapChainImages_.size() );
    for ( size_t i = 0; i < swapChainImages_.size(); i++ ) {
        const VkImageViewCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapChainImages_[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = swapChainImageFormat_,
            .components = { VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY },
            .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                  .baseMipLevel = 0,
                                  .levelCount = 1,
                                  .baseArrayLayer = 0,
                                  .layerCount = 1 } };

        if ( const auto err =
                 vkCreateImageView( device_->handle(), &createInfo, nullptr,
                                    &swapChainImageViews_[i] );
             err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to create swapchain image views with code {}\n!",
                string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
                "vk::Swapchain === image view {} created!", i );
        }
    }
}

void Swapchain::createFramebuffers( const vk::Pipeline *pipeline ) {
    framebuffers_.resize( swapChainImageViews_.size() );
    for ( size_t i = 0; i < swapChainImageViews_.size(); i++ ) {
        std::array<VkImageView, 1> attachments = { swapChainImageViews_[i] };
        const VkFramebufferCreateInfo framebufferInfo{
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = pipeline->renderpass(),
            .attachmentCount = 1,
            .pAttachments = attachments.data(),
            .width = swapChainExtent_.width,
            .height = swapChainExtent_.height,
            .layers = 1 };

        if ( const auto err =
                 vkCreateFramebuffer( device_->handle(), &framebufferInfo,
                                      nullptr, &framebuffers_[i] );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to create framebuffer at {} with code {}!",
                             i, string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
                "vk::Swapchain === framebuffer {} created!", i );
        }
    }
}

}  // namespace tire::vk
