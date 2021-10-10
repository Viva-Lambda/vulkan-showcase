#pragma once
#include <external.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {
//

struct ImageCreateInfo {
  //
private:
  VkStructureType _stype = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  void *_pnext = NULL;

public:
  std::vector<VkImageCreateFlagBits> img_flags;
  std::vector<VkImageUsageFlagBits> usage_flags;
  VkImageType imageType;
  VkFormat format;
  VkExtent3D extent;
  uint32_t mipLevels;
  uint32_t arrayLayers;
  VkSampleCountFlagBits samples;
  VkImageTiling tiling;
  VkImageUsageFlags usage;
  VkSharingMode sharingMode;
  std::vector<std::uint32_t> queue_family_indices;
  VkImageLayout initialLayout;

  VkStructureType stype() const { return _stype; }
  void *pNext() const { return _pnext; }
};

template <> struct StructChecker<ImageCreateInfo> {
  Result_Vk check(const ImageCreateInfo &ici) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
  }
}
} // namespace vtuto
