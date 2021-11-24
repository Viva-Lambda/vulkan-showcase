#pragma once

#include <external.hpp>

using namespace vtuto;

namespace vtuto {
//

/**
  VkImageSubresourceRange structure is defined as:

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
struct ImageSubresourceRange_VK {
  VkImageSubresourceRange srange;

  template <std::uint32_t LayerCount = 1,       //
            std::uint32_t BaseArrayLayer = 0,   //
            std::uint32_t BaseMipLevel = 0,     //
            std::uint32_t LevelCount = 1,       //
            VkImageAspectFlagBits... AspectBits // some aspect bits
            >
  void set() {
    constexpr std::array<VkImageAspectFlagBits, sizeof...(AspectBits)> aflags =
        {{AspectBits...}};
    VkImageAspectFlags afs = aflags[0];
    for (unsigned int i = 1; i < aflags.size(); i++) {
      afs |= aflags[i];
    }
    VkImageSubresourceRange subrange{};
    subrange.aspectMask = afs;
    subrange.baseMipLevel = BaseMipLevel;
    subrange.levelCount = LevelCount;
    subrange.baseArrayLayer = BaseArrayLayer;
    subrange.layerCount = LayerCount;
    srange = subrange;
  }
  ImageSubresourceRange_VK() {}
  ImageSubresourceRange_VK(std::uint32_t layer_count,
                           std::uint32_t base_array_layer,
                           std::uint32_t base_mip_level,
                           std::uint32_t level_count,
                           const std::vector<VkImageAspectFlagBits> &aflags) {
    VkImageAspectFlags afs = aflags[0];
    for (unsigned int i = 1; i < aflags.size(); i++) {
      afs |= aflags[i];
    }
    VkImageSubresourceRange subrange{};
    subrange.aspectMask = afs;
    subrange.baseMipLevel = base_mip_level;
    subrange.levelCount = level_count;
    subrange.baseArrayLayer = base_array_layer;
    subrange.layerCount = layer_count;
    srange = subrange;
  }
};

/**
The VkImageCreateInfo structure is defined as:

typedef struct VkImageViewCreateInfo {
    VkStructureType            sType;
    const void*                pNext;
    VkImageViewCreateFlags     flags;
    VkImage                    image;
    VkImageViewType            viewType;
    VkFormat                   format;
    VkComponentMapping         components;
    VkImageSubresourceRange    subresourceRange;
} VkImageViewCreateInfo;

- sType is the type of this structure.

- pNext is NULL or a pointer to a structure extending this
structure.

- flags is a bitmask of VkImageViewCreateFlagBits describing
additional
parameters of the image view.

- image is a VkImage on which the view will be created.

- viewType is a VkImageViewType value specifying the type of
the image view.

- format is a VkFormat describing the format and type used
to interpret texel
blocks in the image.

- components is a VkComponentMapping structure specifying a
remapping of color
components (or of depth or stencil components after they
have been converted
into color components).

- subresourceRange is a VkImageSubresourceRange structure
selecting the set of
mipmap levels and array layers to be accessible to the view.

 */
struct ImageViewCreateInfo_VK {
  VkImageViewCreateInfo createInfo;

  template <VkImageViewType ImageViewType = VK_IMAGE_VIEW_TYPE_2D, //
            VkFormat ImageFormat = VK_FORMAT_B8G8R8A8_SRGB,        //
            VkComponentSwizzle R = VK_COMPONENT_SWIZZLE_R,         //
            VkComponentSwizzle G = VK_COMPONENT_SWIZZLE_G,         //
            VkComponentSwizzle B = VK_COMPONENT_SWIZZLE_B,         //
            VkComponentSwizzle A = VK_COMPONENT_SWIZZLE_A,         //
            std::uint32_t LayerCount = 1,                          //
            std::uint32_t BaseArrayLayer = 0,                      //
            std::uint32_t BaseMipLevel = 0,                        //
            std::uint32_t LevelCount = 1,                          //
            VkImageAspectFlagBits... AspectBits // some aspect bits
            >
  void set(VkImage &image) {
    ImageSubresourceRange_VK srange;
    srange.set<LayerCount, BaseArrayLayer, BaseMipLevel, LevelCount,
               AspectBits...>();
    VkImageViewCreateInfo _createInfo{};
    _createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    _createInfo.viewType = ImageViewType;
    _createInfo.format = ImageFormat;
    _createInfo.image = image;
    _createInfo.components.r = R;
    _createInfo.components.g = G;
    _createInfo.components.b = B;
    _createInfo.components.a = A;
    _createInfo.subresourceRange = srange.srange;
    createInfo = _createInfo;
  }
  ImageViewCreateInfo_VK() {}

  ImageViewCreateInfo_VK(VkImageViewType image_view_type, //
                         VkComponentSwizzle r, VkComponentSwizzle g,
                         VkComponentSwizzle b, VkComponentSwizzle a,
                         VkImage &image, const VkFormat &image_format,
                         const VkImageSubresourceRange &srange) {
    //
    VkImageViewCreateInfo _createInfo{};
    _createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    _createInfo.viewType = image_view_type;
    _createInfo.format = image_format;
    _createInfo.image = image;
    _createInfo.components.r = r;
    _createInfo.components.g = g;
    _createInfo.components.b = b;
    _createInfo.components.a = a;
    _createInfo.subresourceRange = srange;
    createInfo = _createInfo;
  }
};

} // namespace vtuto
