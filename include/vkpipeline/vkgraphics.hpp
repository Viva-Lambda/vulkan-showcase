// graphics pipeline related functions objects

#pragma once

namespace vtuto {

struct PipelineVertexInputStateCreateInfoVk {

  VkPipelineVertexInputStateCreateInfo createInfo;

  template <typename T> void set(std::optional<T> &pNext) {
    if (pNext.has_value()) {
      auto val = pNext.value();
      if (std::is_pointer(T)) {
        createInfo.pNext = static_cast<void *>(val);
      } else {
        createInfo.pNext = static_cast<void *>(&val);
      }
    }
  }

  PipelineVertexInputStateCreateInfoVk() {
    createInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  }
};

struct PipelineTessellationStateCreateInfoVk {};

struct PipelineViewportStateCreateInfoVk {};

struct PipelineRasterizationStateCreateInfoVk {};

struct PipelineMultisampleStateCreateInfoVk {};

struct PipelineDepthStencilStateCreateInfoVk {};

struct PipelineColorBlendStateCreateInfoVk {};

struct PipelineDynamicStateCreateInfoVk {};

//
struct GraphicsPipelineCreateInfoVk {};

} // namespace vtuto
