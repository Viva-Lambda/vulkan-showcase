#pragma once
// subpass objects to render pass
#include <external.hpp>

namespace vtuto { //

/**
typedef struct VkSubpassDescription {
 VkSubpassDescriptionFlags flags; // compile time
 VkPipelineBindPoint pipelineBindPoint; // compile time
 uint32_t inputAttachmentCount; // compile time
 const VkAttachmentReference* pInputAttachments; // runtime
 uint32_t colorAttachmentCount; //
 const VkAttachmentReference* pColorAttachments;
 const VkAttachmentReference* pResolveAttachments;
 const VkAttachmentReference* pDepthStencilAttachment;
 uint32_t preserveAttachmentCount;
 const uint32_t* pPreserveAttachments;

} VkSubpassDescription;

 */
struct SubpassDescriptionVk {

  VkSubpassDescription subpass;

  template <VkPipelineBindPoint pipelineBindPoint> void set() {
    subpass.pipelineBindPoint = pipelineBindPoint;
  }

  SubpassDescriptionVk(
      std::optional<std::array<VkAttachmentReference>> &colorRefs,
      std::optional<std::array<VkAttachmentReference>> &inputRefs,
      std::optional<VkAttachmentReference> &resolveRef,
      std::optional<VkAttachmentReference> &depthStencilRef,
      std::optional<std::array<std::uint32_t>> &preserveRefs,
      std::optional<std::array<VkSubpassDescriptionFlagBits>> &flagRefs) {
    if (colorRefs.has_value()) {
      auto colorAttachments = colorRefs.value();
      subpass.pColorAttachments = colorAttachments.data();
      subpass.colorAttachmentCount =
          static_cast<std::uint32_t>(colorAttachments.size());
    }
    if (resolveRef.has_value()) {
      subpass.pResolveAttachments = resolveRef.value();
    }
    if (depthStencilRef.has_value()) {
      subpass.pDepthStencilAttachment = depthStencilRef.value();
    }
    if (inputRefs.has_value()) {
      auto inputAttachments = inputRefs.value();
      subpass.pInputAttachments = inputAttachments.data();
      subpass.inputAttachmentCount =
          static_cast<std::uint32_t>(inputAttachments.size());
    }
    if (preserveRefs.has_value()) {
      auto preserveAttachments = preserveRefs.value();
      subpass.pPreserveAttachments = preserveAttachments.data();
      subpass.preserveAttachmentCount =
          static_cast<std::uint32_t>(preserveAttachments.size());
    }
    if (flagRefs.has_value()) {
      auto flags = flagRefs.value();
      VkSubpassDescriptionFlags f = flags[0];
      for (unsigned int i = 1; i < flags.size(); i++) {
        f |= flags[i];
      }
      subpass.flags = f;
    }
  }
};

/**
typedef struct VkSubpassDependency {

uint32_t srcSubpass;

uint32_t dstSubpass;

VkPipelineStageFlags srcStageMask;

VkPipelineStageFlags dstStageMask;

VkAccessFlags srcAccessMask;

VkAccessFlags dstAccessMask;

VkDependencyFlags dependencyFlags;
} VkSubpassDependency;

- srcSubpass is the subpass index of the first subpass in the dependency, or
VK_SUBPASS_EXTERNAL.
- dstSubpass is the subpass index of the second subpass in the dependency, or
VK_SUBPASS_EXTERNAL.
- srcStageMask is a bitmask of VkPipelineStageFlagBits specifying the source
stage mask.
- dstStageMask is a bitmask of VkPipelineStageFlagBits specifying the
destination stage mask
- srcAccessMask is a bitmask of VkAccessFlagBits specifying a source access
mask.
- dstAccessMask is a bitmask of VkAccessFlagBits specifying a destination
access mask.
- dependencyFlags is a bitmask of VkDependencyFlagBits.

*/
struct SubpassDependencyVk {
  VkSubpassDependency dependency;

  template <
      std::uint32_t srcSubpass = VK_SUBPASS_EXTERNAL, //
      std::uint32_t dstSubpass = 0,                   //
      VkPipelineStageFlags srcStageMask =
          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, //
      VkPipelineStageFlags dstStageMask =
          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,                 //
      VkAccessFlags srcAccessMask = 0,                                   //
      VkAccessFlags dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT //
      >
  void set() {
    dependency.srcSubpass = srcSubpass;
    dependency.dstSubpass = dstSubpass;
    dependency.srcStageMask = srcStageMask;
    dependency.dstStageMask = dstStageMask;
    dependency.srcAccessMask = srcAccessMask;
    dependency.dstAccessMask = dstAccessMask;
  }

  SubpassDependencyVk(
      std::optional<std::array<VkDependencyFlagBits>> &flagRef) {
    if (flagRef.has_value()) {
      auto flags = flagRef.value();
      VkDependencyFlags f = flags[0];
      for (unsigned int i = 1; i < flags.size(); i++) {
        f |= flags[i];
      }
      dependency.dependencyFlags = f;
    }
  }
};

}; // namespace vtuto
