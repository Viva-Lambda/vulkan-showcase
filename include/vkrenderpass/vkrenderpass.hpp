#pragma once
// multiview and single view render passes.
#include <external.hpp>
#include <vkutils/litutils.hpp>
#include <vkutils/varia.hpp>

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

typedef std::tuple<
    std::optional<array_vk<VkRenderPassCreateFlagBits>>,
    std::optional<array_vk<VkAttachmentDescription>>,
    std::optional<array_vk<VkSubpassDescription>>,
    std::optional<array_vk<VkSubpassDependency>>>
    RenderPassOpts;

template <>
struct VkStructSetter<VkRenderPassCreateInfo,
                      RenderPassOpts> {
  static void set(VkRenderPassCreateInfo &renderPassInfo,
                  RenderPassOpts &opts) {
    auto flagBitRefs = std::get<0>(opts);
    auto attachmentRefs = std::get<1>(opts);
    auto subpassDescrRefs = std::get<2>(opts);
    auto subpassDepRefs = std::get<3>(opts);
    //
    renderPassInfo.sType =
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    if (flagBitRefs.has_value()) {
      auto flagBits = flagBitRefs.value();
      VkRenderPassCreateFlags f = flagBits.obj()[0];
      unsigned int size =
          static_cast<unsigned int>(flagBits.length());
      for (unsigned int i = 0; i < size; i++) {
        f |= flagBits.obj()[i];
      }
      renderPassInfo.flags = f;
    }
    if (attachmentRefs.has_value()) {
      auto attachments = attachmentRefs.value();
      renderPassInfo.pAttachments = attachments.obj();
      renderPassInfo.attachmentCount = attachments.length();
    }
    if (subpassDescrRefs.has_value()) {
      auto subpasses = subpassDescrRefs.value();
      renderPassInfo.pSubpasses = subpasses.obj();
      renderPassInfo.subpassCount = subpasses.length();
    }
    if (subpassDepRefs.has_value()) {
      auto dependencies = subpassDepRefs.value();
      renderPassInfo.pDependencies = dependencies.obj();
      renderPassInfo.dependencyCount =
          dependencies.length();
    }
  }
};

} // namespace vtuto
