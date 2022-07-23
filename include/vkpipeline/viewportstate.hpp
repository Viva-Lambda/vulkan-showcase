#pragma once

// viewport state create info
#include <external.hpp>
#include <vkutils/varia.hpp>

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
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is reserved for future use.
- viewportCount is the number of viewports used by the
pipeline.
- pViewports is a pointer to an array of VkViewport
structures, defining the
viewport transforms.  If the viewport state is dynamic, this
member is
ignored.
- scissorCount is the number of scissors and must match the
number of
viewports.
- pScissors is a pointer to an array of VkRect2D structures
defining the
rectangular bounds of the scissor for the corresponding
viewport. If the
scissor state is dynamic, this member is ignored.

 */
constexpr void VkFlagSetter(
    VkPipelineViewportStateCreateInfo &createInfo) {

  createInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
}

void VkOptSetter(
    VkPipelineViewportStateCreateInfo &createInfo,
    const std::optional<
        array_vk<std::pair<VkViewport, VkRect2D>>>
        &viewport_scissors_ref = std::nullopt) {
  //
  if (viewport_scissors_ref.has_value()) {
    array_vk<std::pair<VkViewport, VkRect2D>>
        viewport_scissors = viewport_scissors_ref.value();
    const std::uint32_t nb_viewport_scissors =
        static_cast<std::uint32_t>(
            viewport_scissors.length());
    auto viewports =
        std::vector<VkViewport>(nb_viewport_scissors);
    auto scissors =
        std::vector<VkRect2D>(nb_viewport_scissors);
    for (std::uint32_t i = 0; i < nb_viewport_scissors;
         i++) {
      VkViewport vport = viewport_scissors.obj()[i].first;
      VkRect2D scissor = viewport_scissors.obj()[i].second;
      viewports[i] = vport;
      scissors[i] = scissor;
    }
    createInfo.viewportCount = nb_viewport_scissors;
    createInfo.pViewports = viewports.data();
    createInfo.scissorCount = nb_viewport_scissors;
    createInfo.pScissors = scissors.data();
  }
}

} // namespace vtuto
