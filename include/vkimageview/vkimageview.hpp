#pragma once

#include <external.hpp>
#include <vkimageview/vkcomponent.hpp>
#include <vkimageview/vkimsubresource.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

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
struct ImViewCreateInfo {
private:
  VkStructureType _stype = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  void *_pnext = NULL;

public:
  std::vector<VkImageViewCreateFlagBits> imflags;
  VkImage image;
  VkImageCreateInfo info;
  VkImageViewType T;
  VkFormat F;
  VkComponentMapping components;
  VkImageSubresourceRange subresourceRange;

  VkStructureType stype() const { return _stype; }
  void *pNext() const { return _pnext; }
};

template <> struct StructChecker<ImViewCreateInfo> {
  static Result_Vk check(const ImViewCreateInfo &s) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    //
    return vr;
  }
};
} // namespace vtuto
