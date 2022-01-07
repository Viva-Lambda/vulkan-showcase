#pragma once
// multiview and single view render passes.
#include <external.hpp>

namespace vtuto { //

/**
typedef struct VkRenderPassCreateInfo {
    VkStructureType sType;
    const void* pNext;
    VkRenderPassCreateFlags flags;
    uint32_t attachmentCount;
    const VkAttachmentDescription* pAttachments;
    uint32_t subpassCount;
    const VkSubpassDescription* pSubpasses;
    uint32_t dependencyCount;
    const VkSubpassDependency* pDependencies;
} VkRenderPassCreateInfo;

 */
struct RenderPassCreateInfoVk {
  VkRenderPassCreateInfo renderPassInfo;

  template <typename T> void set(std::optional<T> &pNext) {
    if (pNext.has_value()) {
      auto val = pNext.value();
      if (std::is_pointer(T)) {
        renderPassInfo.pNext = static_cast<void *>(val);
      } else {
        renderPassInfo.pNext = static_cast<void *>(&val);
      }
    }
  }

  RenderPassCreateInfoVk(
      std::optional<std::array<VkRenderPassCreateFlagBits>> &flagBitRefs,
      std::optional<std::array<VkAttachmentDescription>> &attachmentRefs,
      std::optional<std::array<VkSubpassDescription>> &subpassDescrRefs,
      std::optional<std::array<VkSubpassDependency>> &subpassDepRefs) {
    //
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    if (flagBitRefs.has_value()) {
      auto flagBits = flagBitRefs.value();
      VkRenderPassCreateFlags f = flagBits[0];
      for (unsigned int i = 0; i < flagBits.size(); i++) {
        f |= flagBits[i];
      }
      renderPassInfo.flags = f;
    }
    if (attachmentRefs.has_value()) {
      auto attachments = attachmentRefs.value();
      renderPassInfo.pAttachments = attachments.data();
      renderPassInfo.attachmentCount = attachments.size();
    }
    if (subpassDescrRefs.has_value()) {
      auto subpasses = subpassDescrRefs.value();
      renderPassInfo.pSubpasses = subpasses.data();
      renderPassInfo.subpassCount = subpasses.size();
    }
    if (subpassDepRefs.has_value()) {
      auto dependencies = subpassDepRefs.value();
      renderPassInfo.pDependencies = dependencies.data();
      renderPassInfo.dependencyCount = dependencies.size();
    }
  }
};
}; // namespace vtuto
