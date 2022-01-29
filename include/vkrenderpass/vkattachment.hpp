#pragma once
// attachment to render pass
#include <external.hpp>
#include <vkutils/varia.hpp>

namespace vtuto {

typedef std::tuple<VkSampleCountFlagBits, VkAttachmentLoadOp,
                   VkAttachmentStoreOp, VkAttachmentLoadOp, VkAttachmentStoreOp,
                   VkImageLayout, VkImageLayout>
    AttachmentDescriptorFlags;

typedef std::tuple<VkFormat> AttachmentDescriptorArgs;

/**
typedef struct VkAttachmentDescription {

VkAttachmentDescriptionFlags flags;

VkFormat format;

VkSampleCountFlagBits samples;

VkAttachmentLoadOp loadOp;

VkAttachmentStoreOp storeOp;

VkAttachmentLoadOp stencilLoadOp;

VkAttachmentStoreOp stencilStoreOp;

VkImageLayout initialLayout;

VkImageLayout finalLayout;
} VkAttachmentDescription;

- flags is a bitmask of VkAttachmentDescriptionFlagBits specifying additional
properties of the attachment.
- format is a VkFormat value specifying the format of the image view that will
be used for the attachment.
- samples is a VkSampleCountFlagBits value specifying the number of samples of
the image.
- loadOp is a VkAttachmentLoadOp value specifying how the contents of color
and depth components of the attachment are treated at the beginning of the
subpass where it is first used.
- storeOp is a VkAttachmentStoreOp value specifying how the contents of color
and depth components of the attachment are treated at the end of the subpass
where it is last used.
- stencilLoadOp is a VkAttachmentLoadOp value specifying how the contents of
stencil components of the attachment are treated at the beginning of the
subpass where it is first used.
- stencilStoreOp is a VkAttachmentStoreOp value specifying how the contents of
stencil components of the attachment are treated at the end of the last
subpass where it is used.
- initialLayout is the layout the attachment image subresource will be in when
a render pass instance begins.
- finalLayout is the layout the attachment image subresource will be
transitioned to when a render pass instance ends.

If the attachment uses a color format, then loadOp and storeOp are used, and
stencilLoadOp and stencilStoreOp are ignored. If the format has depth and/or
stencil components, loadOp and storeOp apply only to the depth data, while
stencilLoadOp and stencilStoreOp define how the stencil data is handled.
loadOp and stencilLoadOp define the load operations that execute as part of
the first subpass that uses the attachment. storeOp and stencilStoreOp define
the store operations that execute as part of the last subpass that uses the
attachment.
 */
template <>
struct VkStructSetter<VkAttachmentDescription,   // ObjType
                      AttachmentDescriptorFlags, // Flags
                      AttachmentDescriptorArgs   // Opts
                      > {
  static void set(VkAttachmentDescription &attachDescr,
                  AttachmentDescriptorFlags &flags,
                  VkFormat &imformat) {
    attachDescr.format = imformat;
    //
    attachDescr.samples = std::get<0>(flags);
    attachDescr.loadOp = std::get<1>(flags);
    attachDescr.storeOp = std::get<2>(flags);
    attachDescr.stencilLoadOp = std::get<3>(flags);
    attachDescr.stencilStoreOp = std::get<4>(flags);
    attachDescr.initialLayout = std::get<5>(flags);
    attachDescr.finalLayout = std::get<6>(flags);
  }
};

/**
typedef struct VkAttachmentReference {

uint32_t attachment;

VkImageLayout layout;
} VkAttachmentReference;

- attachment is either an integer value identifying an attachment at the
corresponding index  VkRenderPassCreateInfo::pAttachments, or
VK_ATTACHMENT_UNUSED to signify that this attachment is not used.

- layout is a VkImageLayout value specifying the layout the attachment uses
during t e subpass.

 */
template <>
struct VkStructSetter<VkAttachmentReference,                   // ObjType
                      std::tuple<std::uint32_t, VkImageLayout> // Flags
                      > {
  static void set(VkAttachmentReference &attachRef,
                  std::tuple<std::uint32_t, VkImageLayout> &flags) {
    attachRef.attachment = std::get<0>(flags);
    attachRef.layout = std::get<1>(flags);
  }
};

} // namespace vtuto
