#pragma once
// attachment to render pass
#include <external.hpp>

namespace vtuto {

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
struct AttachmentDescriptionVk {
  //
  VkAttachmentDescription attachDescr;

  template <
      VkSampleCountFlagBits scfb = VK_SAMPLE_COUNT_1_BIT,                    //
      VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,               //
      VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE,            //
      VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,    //
      VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, //
      VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,               //
      VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR            //
      >
  void set(std::optional<VkFormat &> imformat) { //
    if (imformat.has_value()) {
      attachDescr.format = imformat.value();
    }
    attachDescr.samples = scfb;
    attachDescr.loadOp = loadOp;
    attachDescr.storeOp = storeOp;
    attachDescr.stencilLoadOp = stencilLoadOp;
    attachDescr.stencilStoreOp = stencilStoreOp;
    attachDescr.initialLayout = initialLayout;
    attachDescr.finalLayout = finalLayout;
  }

  AttachmentDescriptionVk(VkFormat &imformat) { attachDescr.format = imformat; }
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
struct AttachmentReferenceVk {
  //
  VkAttachmentReference attachRef;

  template <uint32_t attachment = 0,
            VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL //
            >
  void set() { //
    attachRef.attachment = attachment;
    attachRef.layout = layout;
  }
  AttachmentDescriptionVk() {}
};
}; // namespace vtuto
