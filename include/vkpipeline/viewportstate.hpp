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

template <std::size_t N>
struct VkArraySetter<VkPipelineViewportStateCreateInfo, N,
                     VkViewport, VkRect2D> {
  static void
  set(VkPipelineViewportStateCreateInfo &createInfo,
      const std::array<VkViewport, N> &vports,
      const std::array<VkRect2D, N> &vrects) {
    createInfo.pViewports = vports.data();
    createInfo.pScissors = vrects.data();
    createInfo.viewportCount =
        static_cast<std::uint32_t>(vports.size());
    createInfo.scissorCount =
        static_cast<std::uint32_t>(vrects.size());
  }
};

} // namespace vtuto
