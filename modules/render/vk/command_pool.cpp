
#include <vulkan/vk_enum_string_helper.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMAND_POOL_CPP{ true };

#include "command_pool.h"

namespace tire::vk {

CommandPool::CommandPool( const vk::Device *device )
    : device_{ device } {
    poolInfo_.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo_.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo_.queueFamilyIndex = device->graphicsFamily();

    const auto err = vkCreateCommandPool( device_->handle(), &poolInfo_,
                                          nullptr, &commandPool_ );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create command pool woth code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_COMMAND_POOL_CPP>( "command pool created!" );
    }
}

}  // namespace tire::vk
