
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "surface.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SURFACEVK_CPP{ true };

namespace tire::vk {

Surface::Surface( Display *display, Window window,
                  const vk::Instance *instance )
    : instance_{ instance } {
    VkXlibSurfaceCreateInfoKHR xlibSurfInfo_{};
    xlibSurfInfo_.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfInfo_.dpy = display;
    xlibSurfInfo_.window = window;

    if ( const auto err = vkCreateXlibSurfaceKHR(
             instance_->handle(), &xlibSurfInfo_, nullptr, &surface_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create xlib surface with code {}\n!",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::Surface === xlib surface created!" );
    }
}

Surface::~Surface() {
    vkDestroySurfaceKHR( instance_->handle(), surface_, nullptr );
}

VkSurfaceKHR Surface::handle() const {
    return surface_;
}

}  // namespace tire::vk
