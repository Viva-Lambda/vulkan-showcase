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

- aspectMask is a bitmask of VkImageAspectFlagBits specifying which aspect(s)
of the image are included in the view.
- baseMipLevel is the first mipmap level accessible to the view.
- levelCount is the number of mipmap levels (starting from baseMipLevel)
accessible to the view.
- baseArrayLayer is the first array layer accessible to the view.
- layerCount is the number of array layers (starting from baseArrayLayer)
accessible to the view.
 */
struct ImSubresourceRange {
  std::vector<VkImageAspectFlagBits> aflags;
  std::uint32_t base_mip_level;
  std::uint32_t level_count;
  std::uint32_t base_array_layer;
  std::uint32_t layer_count;
};
bool check_memory_plane(const std::vector<VkImageAspectFlagBits> &vs) {

  auto check_mplane = [](VkImageAspectFlagBits a) {
    if (a == VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT) {
      return true;
    }
    if (a == VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT) {
      return true;
    }
    if (a == VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT) {
      return true;
    }
    if (a == VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT) {
      return true;
    }
    return false;
  };
  if (std::any_of(vs.begin(), vs.end(), check_mplane)) {
    return false;
  }
  return true;
}
bool check_amask_content(const std::vector<VkImageAspectFlagBits> &vs) {
  //
  auto check_acolor = [](VkImageAspectFlagBits a) {
    return a == VK_IMAGE_ASPECT_COLOR_BIT;
  };
  auto check_aplane = [](VkImageAspectFlagBits a) {
    if (a == VK_IMAGE_ASPECT_PLANE_0_BIT) {
      return true;
    }
    if (a == VK_IMAGE_ASPECT_PLANE_1_BIT) {
      return true;
    }
    if (a == VK_IMAGE_ASPECT_PLANE_2_BIT) {
      return true;
    }
    return false;
  };

  auto facolor = std::any_of(vs.begin(), vs.end(), check_acolor);
  auto faplane = std::any_of(vs.begin(), vs.end(), check_aplane);
  if (faplane && faplane) {
    return false;
  }
  return true;
}

/**
  Explicit Valid Usage:

- If levelCount is not VK_REMAINING_MIP_LEVELS, it must be greater than 0
- If layerCount is not VK_REMAINING_ARRAY_LAYERS, it must be greater than 0
- If aspectMask includes VK_IMAGE_ASPECT_COLOR_BIT, then it must not include
any of VK_IMAGE_ASPECT_PLANE_0_BIT, VK_IMAGE_ASPECT_PLANE_1_BIT, or
VK_IMAGE_ASPECT_PLANE_2_BIT
- aspectMask must not include VK_IMAGE_ASPECT_MEMORY_PLANE_i_BIT_EXT for any
index i

Implicit Valid Usage:

- VUID-VkImageSubresourceRange-aspectMask-parameter
aspectMask must be a valid combination of VkImageAspectFlagBits values

- VUID-VkImageSubresourceRange-aspectMask-requiredbitmask
aspectMask must not be 0
*/
template <> struct StructChecker<ImSubresourceRange> {
  static Result_Vk check(const ImSubresourceRange &s) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    if (layer_count != VK_REMAINING_ARRAY_LAYERS) {
      if (layer_count <= 0) {
        vr.status = IMAGE_SUBRESOURCE_RANGE_ERROR_VK;
        vr.spec_info = R"(
VUID-VkImageSubresourceRange-layerCount-01721 ::
If layerCount is not VK_REMAINING_ARRAY_LAYERS, it must be greater than 0
)";
        return vr;
      }
    }
    if (level_count != VK_REMAINING_MIP_LEVELS) {
      if (level_count <= 0) {
        vr.status = IMAGE_SUBRESOURCE_RANGE_ERROR_VK;
        vr.spec_info = R"(
VUID-VkImageSubresourceRange-levelCount-01720 ::
If levelCount is not VK_REMAINING_MIP_LEVELS, it must be greater than 0
)";
        return vr;
      }
    }
    if (aflags.size() == 0) {
      vr.status = IMAGE_SUBRESOURCE_RANGE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageSubresourceRange-aspectMask-requiredbitmask ::
aspectMask must not be 0
)";
      return vr;
    }
    if (!check_memory_plane(aflags)) {
      vr.status = IMAGE_SUBRESOURCE_RANGE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageSubresourceRange-aspectMask-02278 ::
aspectMask must not include VK_IMAGE_ASPECT_MEMORY_PLANE_i_BIT_EXT for any
index i
)";
      return vr;
    }
    if (!check_amask_content(aflags)) {
      vr.status = IMAGE_SUBRESOURCE_RANGE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageSubresourceRange-aspectMask-01670
If aspectMask includes VK_IMAGE_ASPECT_COLOR_BIT, then it must not include any
of VK_IMAGE_ASPECT_PLANE_0_BIT, VK_IMAGE_ASPECT_PLANE_1_BIT, or
VK_IMAGE_ASPECT_PLANE_2_BIT
)";
      return vr;
    }
    return vr;
  }
};

Result_Vk createSubresourceRange(const ImSubresourceRange &ims,
                                 VkImageSubresourceRange &srange) {
  //
  Result_Vk vr = StructChecker<ImSubresourceRange>::check(ims);
  if (vr.status != SUCCESS_OP) {
    return vr;
  }
  srange.layerCount = ims.layer_count;
  srange.baseArrayLayer = ims.base_array_layer;
  //
  srange.baseMipLevel = ims.base_mip_level;
  srange.levelCount = ims.level_count;
  //
  VkImageAspectFlags afs = ims.aflags[0];
  for (unsigned int i = 1; i < ims.aflags.size(); i++) {
    afs |= ims.aflags[i];
  }
  srange.aspectMask = afs;
  return vr;
}

} // namespace vtuto
