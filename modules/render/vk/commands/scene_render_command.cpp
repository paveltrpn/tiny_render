
#include "scene_render_command.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_RENDER_COMMAND_CPP{ true };

namespace tire::vk {

SceneRenderCommand::SceneRenderCommand( const vk::Device *device,
                                        const vk::Pipeline *pipeline,
                                        const CommandPool *pool,
                                        int commandsCount )
    : device_{ device }
    , pipeline_{ pipeline }
    , pool_{ pool } {
    commandsRange_.reserve( commandsCount );
    for ( size_t i{ 0 }; i < commandsCount; ++i ) {
        const VkCommandBufferAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = pool_->handle(),
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1 };

        VkCommandBuffer buffer{ VK_NULL_HANDLE };
        if ( const auto err = vkAllocateCommandBuffers( device_->handle(),
                                                        &allocInfo, &buffer );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to allocate command buffers with code {}!",
                             string_VkResult( err ) ) );
        } else {
            commandsRange_.push_back( buffer );
            log::debug<DEBUG_OUTPUT_SCENE_RENDER_COMMAND_CPP>(
                "vk::SceneRenderCommand === buffer created!" );
        };
    }
    const auto cc = Colorf{ "darkblue" };
    clearColor_.color = { cc.r(), cc.g(), cc.b(), cc.a() },
    clearColor_.depthStencil = { .depth = 0.0f, .stencil = 0 };

    width_ = device_->extent().width;
    height_ = device_->extent().height;
}

void SceneRenderCommand::clean() {
    vkFreeCommandBuffers( device_->handle(), pool_->handle(),
                          commandsRange_.size(), commandsRange_.data() );
}

void SceneRenderCommand::reset( size_t id ) {
    vkResetCommandBuffer( commandsRange_[id], 0 );
}

void SceneRenderCommand::prepare( size_t id, VkFramebuffer framebuffer,
                                  algebra::matrix4f view,
                                  algebra::vector3f color, VkBuffer vbo,
                                  uint32_t vCount ) {
    const VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr };
    // NOTE: omit return code check
    vkBeginCommandBuffer( commandsRange_[id], &beginInfo );

    const VkRenderPassBeginInfo renderPassInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = pipeline_->renderpass(),
        .framebuffer = framebuffer,
        .renderArea = { .offset = { .x = 0, .y = 0 },
                        .extent = { device_->extent() } },
        .clearValueCount = 1,
        .pClearValues = &clearColor_ };

    vkCmdBeginRenderPass( commandsRange_[id], &renderPassInfo,
                          VK_SUBPASS_CONTENTS_INLINE );

    // Dynamic viewport. No performance penalty.
    // Take out work from pipeline creation.
    const VkViewport viewport{ .x = 0.0f,
                               .y = 0.0f,
                               .width = static_cast<float>( width_ ),
                               .height = static_cast<float>( height_ ),
                               .minDepth = 0.0f,
                               .maxDepth = 1.0f };
    vkCmdSetViewport( commandsRange_[id], 0, 1, &viewport );

    const VkRect2D scissor{ { .x = 0, .y = 0 },
                            { .width = width_, .height = height_ } };
    vkCmdSetScissor( commandsRange_[id], 0, 1, &scissor );

    vkCmdBindPipeline( commandsRange_[id], VK_PIPELINE_BIND_POINT_GRAPHICS,
                       pipeline_->pipeline() );
    vkCmdPushConstants( commandsRange_[id], pipeline_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( view ), &view );
    vkCmdPushConstants( commandsRange_[id], pipeline_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, sizeof( view ),
                        sizeof( color ), &color );

    std::array<VkBuffer, 1> vertexBuffers = { vbo };
    std::array<VkDeviceSize, 1> offsets = { 0 };
    vkCmdBindVertexBuffers( commandsRange_[id], 0, 1, vertexBuffers.data(),
                            offsets.data() );
    vkCmdDraw( commandsRange_[id], vCount, 3, 0, 0 );

    vkCmdEndRenderPass( commandsRange_[id] );
    // NOTE: omit return code check
    vkEndCommandBuffer( commandsRange_[id] );
}

void SceneRenderCommand::submit( VkSemaphore waitSemaphores,
                                 VkSemaphore signalSemaphores, VkFence fence ) {
    std::array<VkPipelineStageFlags, 1> waitStages = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    std::array<VkSemaphore, 1> waitsems{ waitSemaphores };
    std::array<VkSemaphore, 1> sgnlsems{ signalSemaphores };

    const VkSubmitInfo submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitsems.data(),
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = static_cast<uint32_t>( commandsRange_.size() ),
        .pCommandBuffers = commandsRange_.data(),
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = sgnlsems.data() };

    // NOTE: omit return code check
    vkQueueSubmit( device_->graphicsQueue(),
                   static_cast<uint32_t>( commandsRange_.size() ), &submitInfo,
                   fence );
}

}  // namespace tire::vk
