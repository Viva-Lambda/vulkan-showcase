#pragma once

// viewport state create info
#include <external.hpp>

namespace vtuto {
/**
typedef struct VkPipelineViewportStateCreateInfo {

VkStructureType sType;

const void* pNext;

VkPipelineViewportStateCreateFlags flags;

uint32_t viewportCount;

const VkViewport* pViewports;

uint32_t scissorCount;

const VkRect2D* pScissors;
} VkPipelineViewportStateCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this structure.
- flags is reserved for future use.
- viewportCount is the number of viewports used by the pipeline.
- pViewports is a pointer to an array of VkViewport structures, defining the
viewport transforms.  If the viewport state is dynamic, this member is
ignored.
- scissorCount is the number of scissors and must match the number of
viewports.
- pScissors is a pointer to an array of VkRect2D structures defining the
rectangular bounds of the scissor for the corresponding viewport. If the
scissor state is dynamic, this member is ignored.

 */
struct PipelineViewportStateCreateInfoVk {
  VkPipelineViewportStateCreateInfo createInfo;

  PipelineViewportStateCreateInfoVk() {}

  PipelineViewportStateCreateInfoVk(
      std::optional<std::array<VkViewport>> &viewportRefs,
      std::optional<std::array<VkRect2D>> &scissorRefs) {
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    if (viewportRefs.has_value()) {
      auto viewports = viewportRefs.value();
      createInfo.pViewports = viewports.data();
      createInfo.viewportCount = viewports.size();
    }
    if (scissorRefs.has_value()) {
      auto scissors = scissorRefs.value();
      createInfo.pScissors = scissors.data();
      createInfo.scissorCount = scissors.size();
    }
  }
};
} // namespace vtuto
