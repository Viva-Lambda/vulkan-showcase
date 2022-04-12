#pragma once

#include <external.hpp>

using namespace vtuto;

namespace vtuto {
//

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
constexpr void
VkFlagSetter(VkImageViewCreateInfo &createInfo,
             const VkImageViewType &vtype) {
  createInfo.sType =
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createInfo.viewType = vtype;
}

void VkArgSetter(VkImageViewCreateInfo &createInfo,
                 VkImage &simage, 
                 VkFormat &simage_format,
                 VkImageSubresourceRange &subRange) {
    //
      createInfo.image = simage;
      createInfo.format = simage_format;
      createInfo.subresourceRange = subRange;
}

} // namespace vtuto
