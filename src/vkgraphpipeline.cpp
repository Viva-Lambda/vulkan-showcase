// main file
#include <hellotriangle.hpp>

using namespace vtuto;

namespace vtuto {
//

void HelloTriangle::createGraphicsPipeline() {
  auto vxShaderCode =
      read_shader_file("./shaders/vulkansimple/vulkansimple.vert.spv");
  auto fragShaderCode =
      read_shader_file("./shaders/vulkansimple/vulkansimple.frag.spv");

  auto vertexModule = createShaderModule(vxShaderCode);
  auto fragModule = createShaderModule(fragShaderCode);
  //
  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertexModule;
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragModule;
  fragShaderStageInfo.pName = "main";

  //
  VkPipelineShaderStageCreateInfo stages[] = {vertShaderStageInfo,
                                              fragShaderStageInfo};

  // vertex input pipeline creation
  VkPipelineVertexInputStateCreateInfo vxInputInfo{};
  vxInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  auto bindingDescr = Vertex::getBindingDescription();
  auto attrDescr = Vertex::getAttributeDescriptions();
  vxInputInfo.vertexBindingDescriptionCount = 1;
  vxInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attrDescr.size());
  vxInputInfo.pVertexBindingDescriptions = &bindingDescr;
  vxInputInfo.pVertexAttributeDescriptions = attrDescr.data();

  // input assembly pipeline creation
  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  // viewport configuration
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)swap_chain.sextent.width;
  viewport.height = (float)swap_chain.sextent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  // scissoring area configuration
  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swap_chain.sextent;

  // viewport state change configuration
  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  // rasterization state configuration
  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;

  // multisample state configuration
  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  // depth attachment state configuration
  VkPipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.minDepthBounds = 0.0f;
  depthStencil.maxDepthBounds = 1.0f;
  depthStencil.stencilTestEnable = VK_FALSE;
  depthStencil.front = {};
  depthStencil.back = {};

  // color blend attachement state configuration
  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;

  //
  // color blend state configuration
  VkPipelineColorBlendStateCreateInfo colorBlend{};
  colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlend.logicOpEnable = VK_FALSE;
  colorBlend.logicOp = VK_LOGIC_OP_COPY;
  colorBlend.attachmentCount = 1;
  colorBlend.pAttachments = &colorBlendAttachment;
  colorBlend.blendConstants[0] = 0.0f;
  colorBlend.blendConstants[1] = 0.0f;
  colorBlend.blendConstants[2] = 0.0f;
  colorBlend.blendConstants[3] = 0.0f;

  // pipeline layout create info configuration
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  //
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &descriptor_set_layout;
  //
  // pipelineLayoutInfo.pushConstantRangeCount = 0;

  CHECK_VK2(vkCreatePipelineLayout(logical_dev.device(), &pipelineLayoutInfo,
                                   nullptr, &pipeline_layout),
            "failed to create pipeline layout");

  // create pipeline object
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = stages;
  pipelineInfo.pVertexInputState = &vxInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = &depthStencil;
  pipelineInfo.pColorBlendState = &colorBlend;
  pipelineInfo.layout = pipeline_layout;
  pipelineInfo.renderPass = render_pass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  CHECK_VK2(vkCreateGraphicsPipelines(logical_dev.device(), VK_NULL_HANDLE, 1,
                                      &pipelineInfo, nullptr,
                                      &graphics_pipeline),
            "failed to create graphics pipeline");

  vkDestroyShaderModule(logical_dev.device(), fragModule, nullptr);
  vkDestroyShaderModule(logical_dev.device(), vertexModule, nullptr);
}
} // namespace vtuto
