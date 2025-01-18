
#include <vulkan/vk_enum_string_helper.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMAND_POOL_CPP{ true };

#include "image/color.h"
#include "commands.h"

namespace tire::vk {

CommandBuffer::CommandBuffer( const vk::Device *device,
                              const CommandPool *pool )
    : device_{ device } {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool->handle();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if ( const auto err = vkAllocateCommandBuffers(
             device_->handle(), &allocInfo, &commandBuffer_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to allocate command buffers with code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_COMMAND_POOL_CPP>( "command buffer created!" );
    };
}

void CommandBuffer::reset() {
    vkResetCommandBuffer( commandBuffer_, 0 );
}

void CommandBuffer::beginRenderPassCommand( VkFramebuffer framebuffer,
                                            const vk::Pipeline *pipeline ) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                   // Optional
    beginInfo.pInheritanceInfo = nullptr;  // Optional

    // NOTE: omit return code check
    vkBeginCommandBuffer( commandBuffer_, &beginInfo );

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = pipeline->renderpass();
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = { .x = 0, .y = 0 };
    renderPassInfo.renderArea.extent = device_->extent();

    const auto clearColor = Colorf{ "darkblue" };
    const VkClearValue clearColorValue = {
        { { clearColor.r(), clearColor.g(), clearColor.b(),
            clearColor.a() } } };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColorValue;

    vkCmdBeginRenderPass( commandBuffer_, &renderPassInfo,
                          VK_SUBPASS_CONTENTS_INLINE );

    vkCmdBindPipeline( commandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                       pipeline->pipeline() );

    // Dynamic viewport. No performance penalty.
    // Take out work from pipeline creation.
    const auto width = device_->extent().width;
    const auto height = device_->extent().height;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>( width );
    viewport.height = static_cast<float>( height );
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport( commandBuffer_, 0, 1, &viewport );

    VkRect2D scissor{};
    scissor.offset = { .x = 0, .y = 0 };
    scissor.extent = { .width = width, .height = height };
    vkCmdSetScissor( commandBuffer_, 0, 1, &scissor );

    // vertexCount: Even though we don’t have a vertex buffer, we technically
    // still have 3 vertices to draw.

    // instanceCount: Used for instanced rendering, use 1 if you’re not doing
    // that.

    // firstVertex: Used as an offset into the vertex buffer, defines the lowest
    // value of gl_VertexIndex.

    // firstInstance: Used as an offset for instanced rendering, defines the
    // lowest value of gl_InstanceIndex.
    vkCmdDraw( commandBuffer_, 9, 3, 0, 0 );

    vkCmdEndRenderPass( commandBuffer_ );

    // NOTE: omit return code check
    vkEndCommandBuffer( commandBuffer_ );
}

void CommandBuffer::submit( const std::vector<VkSemaphore> &waitSemaphores,
                            const std::vector<VkSemaphore> &signalSemaphores,
                            VkFence fence, VkQueue queue ) {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    std::array<VkPipelineStageFlags, 1> waitStages = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStages.data();
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer_;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores.data();

    // NOTE: omit return code check
    vkQueueSubmit( queue, 1, &submitInfo, fence );
}

}  // namespace tire::vk
