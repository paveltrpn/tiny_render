
#include <vulkan/vk_enum_string_helper.h>
#include <vector>

#include "pipeline.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_PIPELINE_CPP{ true };
#include "config/config.h"

namespace tire::vk {

Pipeline::Pipeline( const vk::Device *device )
    : device_{ device } {
    shaderStorage_ = std::make_unique<vk::ShaderStorage>( device_ );
    renderpass_ = std::make_unique<vk::RenderpassSimple>( device_ );
}

Pipeline::~Pipeline() {
    vkDestroyPipeline( device_->handle(), pipeline_, nullptr );
}

void PiplineSimple::initFixed() {
    // VkPipelineVertexInputStateCreateInfo
    vertexInput_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInput_.vertexBindingDescriptionCount = 0;
    vertexInput_.pVertexBindingDescriptions = nullptr;  // Optional
    vertexInput_.vertexAttributeDescriptionCount = 0;
    vertexInput_.pVertexAttributeDescriptions = nullptr;
    vertexInput_.pNext = nullptr;
    vertexInput_.flags = 0;

    // VkPipelineInputAssemblyStateCreateInfo
    inputAssembly_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly_.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly_.primitiveRestartEnable = VK_FALSE;
    inputAssembly_.pNext = nullptr;
    inputAssembly_.flags = 0;

    /*
    const auto width = device_->extent().width;
    const auto height = device_->extent().height;

    // VkViewport
    viewport_.x = 0.0f;
    viewport_.y = 0.0f;
    viewport_.width = static_cast<float>( width );
    viewport_.height = static_cast<float>( height );
    viewport_.minDepth = 0.0f;
    viewport_.maxDepth = 1.0f;

    // VkRect2D
    scissor_.offset = { .x = 0, .y = 0 };
    scissor_.extent = { .width = width, .height = height };  //swapChainExtent;

    // VkPipelineViewportStateCreateInfo
    viewportState_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState_.viewportCount = 1;
    viewportState_.pViewports = &viewport_;
    viewportState_.scissorCount = 1;
    viewportState_.pScissors = &scissor_;
    viewportState_.pNext = nullptr;
    viewportState_.flags = 0;
    */

    // VkPipelineRasterizationStateCreateInfo
    rasterizer_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer_.depthClampEnable = VK_FALSE;
    rasterizer_.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer_.lineWidth = 1.0f;
    rasterizer_.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer_.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer_.depthBiasEnable = VK_FALSE;
    rasterizer_.depthBiasConstantFactor = 0.0f;  // Optional
    rasterizer_.depthBiasClamp = 0.0f;           // Optional
    rasterizer_.depthBiasSlopeFactor = 0.0f;     // Optional
    rasterizer_.pNext = nullptr;
    rasterizer_.flags = 0;

    // VkPipelineMultisampleStateCreateInfo
    multisampling_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling_.sampleShadingEnable = VK_FALSE;
    multisampling_.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling_.minSampleShading = 1.0f;           // Optional
    multisampling_.pSampleMask = nullptr;             // Optional
    multisampling_.alphaToCoverageEnable = VK_FALSE;  // Optional
    multisampling_.alphaToOneEnable = VK_FALSE;       // Optional
    multisampling_.pNext = nullptr;
    multisampling_.flags = 0;

    // VkPipelineColorBlendAttachmentState
    colorBlendAttachment_.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment_.blendEnable = VK_FALSE;
    colorBlendAttachment_.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment_.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment_.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment_.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment_.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment_.alphaBlendOp = VK_BLEND_OP_ADD;  // Optional
    colorBlendAttachment_.blendEnable = VK_TRUE;
    colorBlendAttachment_.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment_.dstColorBlendFactor =
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment_.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment_.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment_.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment_.alphaBlendOp = VK_BLEND_OP_ADD;

    // VkPipelineColorBlendStateCreateInfo
    colorBlending_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending_.logicOpEnable = VK_FALSE;
    colorBlending_.logicOp = VK_LOGIC_OP_COPY;  // Optional
    colorBlending_.attachmentCount = 1;
    colorBlending_.pAttachments = &colorBlendAttachment_;
    colorBlending_.blendConstants[0] = 0.0f;  // Optional
    colorBlending_.blendConstants[1] = 0.0f;  // Optional
    colorBlending_.blendConstants[2] = 0.0f;  // Optional
    colorBlending_.blendConstants[3] = 0.0f;  // Optional
    colorBlending_.pNext = nullptr;
    colorBlending_.flags = 0;

    // VkPipelineDynamicStateCreateInfo
    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT,
                                                  VK_DYNAMIC_STATE_LINE_WIDTH };
    dynamicState_.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState_.dynamicStateCount =
        static_cast<uint32_t>( dynamicStates.size() );
    dynamicState_.pDynamicStates = dynamicStates.data();
    dynamicState_.pNext = nullptr;
    dynamicState_.flags = 0;
}

void PiplineSimple::initProgable(
    const std::vector<std::filesystem::path> &files ) {
    shaderStorage_->fill( files );

    // VkPipelineShaderStageCreateInfo
    vertShaderStage_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStage_.pNext = nullptr;
    vertShaderStage_.flags = 0;
    vertShaderStage_.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStage_.module = shaderStorage_->get<ShaderStageType::VERTEX>();
    vertShaderStage_.pName = "main";

    // VkPipelineShaderStageCreateInfo
    fragShaderStage_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStage_.pNext = nullptr;
    vertShaderStage_.flags = 0;
    fragShaderStage_.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStage_.module = shaderStorage_->get<ShaderStageType::FRAGMENT>();
    fragShaderStage_.pName = "main";
}

VkPipelineLayout PiplineSimple::initLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;             // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr;          // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0;     // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr;  // Optional

    VkPipelineLayout layout{ VK_NULL_HANDLE };

    if ( const auto err = vkCreatePipelineLayout(
             device_->handle(), &pipelineLayoutInfo, nullptr, &layout );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create pipeline layout with code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::PipelineSimple === pipeline layout created!" );
    }

    return layout;
}

void PiplineSimple::initPipeline() {
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;

    // NOTE: Will vulkan ignore nullptr shader stages?
    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {
        vertShaderStage_, fragShaderStage_ };

    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInput_;
    pipelineInfo.pInputAssemblyState = &inputAssembly_;
    pipelineInfo.pViewportState = nullptr;  //&viewportState_;
    pipelineInfo.pRasterizationState = &rasterizer_;
    pipelineInfo.pMultisampleState = &multisampling_;
    pipelineInfo.pDepthStencilState = nullptr;  // Optional
    pipelineInfo.pColorBlendState = &colorBlending_;
    pipelineInfo.pDynamicState = nullptr;  // Optional

    const auto layout = initLayout();
    pipelineInfo.layout = layout;

    pipelineInfo.renderPass = renderpass_->handle();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;  // Optional
    pipelineInfo.basePipelineIndex = -1;               // Optional

    if ( const auto err =
             vkCreateGraphicsPipelines( device_->handle(), VK_NULL_HANDLE, 1,
                                        &pipelineInfo, nullptr, &pipeline_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create graphics pipeline with code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::PipelineSimple === graphics pipeline created!" );
    }

    // There is no need to store this handle after pipeline creation and pass
    // it to pipeline itself. It can be safeley removed after pipeline creation
    // and pipelines thoose uses this pipeline layout stay valid.
    vkDestroyPipelineLayout( device_->handle(), layout, nullptr );
}

}  // namespace tire::vk
