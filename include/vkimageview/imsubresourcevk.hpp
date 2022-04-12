#pragma once
#include <external.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {
/**
typedef struct VkImageSubresourceRange {
    VkImageAspectFlags    aspectMask;
    uint32_t              baseMipLevel;
    uint32_t              levelCount;
    uint32_t              baseArrayLayer;
    uint32_t              layerCount;
} VkImageSubresourceRange;

- aspectMask is a bitmask of VkImageAspectFlagBits
specifying which aspect(s)
of the image are included in the view.
- baseMipLevel is the first mipmap level accessible to the
view.
- levelCount is the number of mipmap levels (starting from
baseMipLevel)
accessible to the view.
- baseArrayLayer is the first array layer accessible to the
view.
- layerCount is the number of array layers (starting from
baseArrayLayer)
accessible to the view.

Explicit Valid Usage:

- If levelCount is not VK_REMAINING_MIP_LEVELS, it must be
greater than 0
- If layerCount is not VK_REMAINING_ARRAY_LAYERS, it must be
greater than 0
- If aspectMask includes VK_IMAGE_ASPECT_COLOR_BIT, then it
must not include
any of VK_IMAGE_ASPECT_PLANE_0_BIT,
VK_IMAGE_ASPECT_PLANE_1_BIT, or
VK_IMAGE_ASPECT_PLANE_2_BIT
- aspectMask must not include
VK_IMAGE_ASPECT_MEMORY_PLANE_i_BIT_EXT for any
index i

Implicit Valid Usage:

- VUID-VkImageSubresourceRange-aspectMask-parameter
aspectMask must be a valid combination of
VkImageAspectFlagBits values

- VUID-VkImageSubresourceRange-aspectMask-requiredbitmask
aspectMask must not be 0
 */
constexpr void
VkFlagSetter(VkImageSubresourceRange &subRange,
             const VkImageAspectFlags &amask,
             const std::uint32_t &base_mip_level,
             const std::uint32_t &level_count,
             const std::uint32_t &base_array_layer,
             const std::uint32_t &layer_count) {
  //
  subRange.aspectMask = amask;
  subRange.baseMipLevel = base_mip_level;
  subRange.levelCount = level_count;
  subRange.baseArrayLayer = base_array_layer;
  subRange.layerCount = layer_count;
}

} // namespace vtuto
