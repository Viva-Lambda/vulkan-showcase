#pragma once
// subpass objects to render pass
#include <external.hpp>
#include <vkutils/litutils.hpp>
#include <vkutils/varia.hpp>

namespace vtuto { //

/**
typedef struct VkSubpassDescription {
 VkSubpassDescriptionFlags flags; // compile time
 VkPipelineBindPoint pipelineBindPoint; // compile time
 uint32_t inputAttachmentCount; // compile time
 const VkAttachmentReference* pInputAttachments; // compile
time, see s*
 uint32_t colorAttachmentCount; //
 const VkAttachmentReference* pColorAttachments; // compile
time, see s*
 const VkAttachmentReference* pResolveAttachments; //
compile time, see s*. The number of this attribute is
colorAttachmentCount
 const VkAttachmentReference* pDepthStencilAttachment; //
compile time probably
 uint32_t preserveAttachmentCount; //
 const uint32_t* pPreserveAttachments; // unsure

} VkSubpassDescription;

s*: spec states that color and input attachments depend on
fragment shader.
Since the fragment shaders are not generated at runtime in
general it can be
assumed that these attachment values are known at compile
time to the
developer.
 */
typedef std::tuple<
    std::optional<array_vk<VkAttachmentReference>>,
    std::optional<array_vk<VkAttachmentReference>>,
    std::optional<VkAttachmentReference>,
    std::optional<VkAttachmentReference>,
    std::optional<const_uint32s>,
    std::optional<array_vk<VkSubpassDescriptionFlagBits>>>
    SubpassDescriptionsOpts;

typedef std::tuple<VkPipelineBindPoint>
    SubpassDescriptionFlags;

template <>
struct VkStructSetter<VkSubpassDescription,
                      SubpassDescriptionsOpts,
                      SubpassDescriptionFlags> {
  static void set(VkSubpassDescription &subpass,
                  SubpassDescriptionsOpts &opts,
                  SubpassDescriptionFlags &pbp) {
    //
    auto colorRefs = std::get<0>(opts);
    auto inputRefs = std::get<1>(opts);
    auto resolveRef = std::get<2>(opts);
    auto depthStencilRef = std::get<3>(opts);
    auto preserveRefs = std::get<4>(opts);
    auto flagRefs = std::get<5>(opts);
    //
    if (colorRefs.has_value()) {
      auto colorAttachments = colorRefs.value();
      subpass.pColorAttachments = colorAttachments.obj();
      subpass.colorAttachmentCount =
          static_cast<std::uint32_t>(
              colorAttachments.length());
    }
    if (resolveRef.has_value()) {
      subpass.pResolveAttachments = &resolveRef.value();
    }
    if (depthStencilRef.has_value()) {
      subpass.pDepthStencilAttachment =
          &depthStencilRef.value();
    }
    if (inputRefs.has_value()) {
      auto inputAttachments = inputRefs.value();
      subpass.pInputAttachments = inputAttachments.obj();
      subpass.inputAttachmentCount =
          static_cast<std::uint32_t>(
              inputAttachments.length());
    }
    if (preserveRefs.has_value()) {
      auto preserveAttachments = preserveRefs.value();
      subpass.pPreserveAttachments =
          preserveAttachments.obj();
      subpass.preserveAttachmentCount =
          static_cast<std::uint32_t>(
              preserveAttachments.length());
    }
    if (flagRefs.has_value()) {
      auto flags = flagRefs.value();
      VkSubpassDescriptionFlags f = flags.obj()[0];
      unsigned int size =
          static_cast<unsigned int>(flags.length());
      for (unsigned int i = 1; i < size; i++) {
        f |= flags.obj()[i];
      }
      subpass.flags = f;
    }
    subpass.pipelineBindPoint = std::get<0>(pbp);
  }
};

constexpr void VkOptSetter(
    VkSubpassDescription &subpass,
    const std::optional<array_vk<VkAttachmentReference>>
        &colorRefs,
    const std::optional<array_vk<VkAttachmentReference>>
        &inputRefs,
    const std::optional<array_vk<VkAttachmentReference>>
        &resolveRef,
    const std::optional<array_vk<VkAttachmentReference>>
        &depthStencilRef,
    const std::optional<const_uint32s> &preserveRefs,
    const std::optional<
        array_vk<VkSubpassDescriptionFlagBits>> &flagRefs) {
  if (colorRefs.has_value()) {
    auto colorAttachments = colorRefs.value();
    subpass.pColorAttachments = colorAttachments.obj();
    subpass.colorAttachmentCount =
        static_cast<std::uint32_t>(
            colorAttachments.length());
  }
  if (resolveRef.has_value()) {
    auto resolveAttachments = resolveRef.value();
    subpass.pResolveAttachments = resolveAttachments.obj();
  }
  if (depthStencilRef.has_value()) {
    auto depthStencilAttachment = depthStencilRef.value();
    subpass.pDepthStencilAttachment =
        depthStencilAttachment.obj();
  }
  if (inputRefs.has_value()) {
    auto inputAttachments = inputRefs.value();
    subpass.pInputAttachments = inputAttachments.obj();
    subpass.inputAttachmentCount =
        static_cast<std::uint32_t>(
            inputAttachments.length());
  }
  if (preserveRefs.has_value()) {
    auto preserveAttachments = preserveRefs.value();
    subpass.pPreserveAttachments =
        preserveAttachments.obj();
    subpass.preserveAttachmentCount =
        static_cast<std::uint32_t>(
            preserveAttachments.length());
  }
  if (flagRefs.has_value()) {
    auto flags = flagRefs.value();
    VkSubpassDescriptionFlags f = flags.obj()[0];
    unsigned int size =
        static_cast<unsigned int>(flags.length());
    for (unsigned int i = 1; i < size; i++) {
      f |= flags.obj()[i];
    }
    subpass.flags = f;
  }
}

constexpr void
VkFlagSetter(VkSubpassDescription &subpass,
             const VkPipelineBindPoint &pbp) {
  //
  subpass.pipelineBindPoint = pbp;
}

typedef std::tuple<
    std::uint32_t, std::uint32_t, VkPipelineStageFlags,
    VkPipelineStageFlags, VkAccessFlags, VkAccessFlags>
    SubpassDependencyFlags;

typedef std::tuple<
    std::optional<array_vk<VkDependencyFlagBits>>>
    SubpassDependencyOpts;

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

- srcSubpass is the subpass index of the first subpass in
the dependency, or
VK_SUBPASS_EXTERNAL.
- dstSubpass is the subpass index of the second subpass in
the dependency, or
VK_SUBPASS_EXTERNAL.
- srcStageMask is a bitmask of VkPipelineStageFlagBits
specifying the source
stage mask.
- dstStageMask is a bitmask of VkPipelineStageFlagBits
specifying the
destination stage mask
- srcAccessMask is a bitmask of VkAccessFlagBits specifying
a source access
mask.
- dstAccessMask is a bitmask of VkAccessFlagBits specifying
a destination
access mask.
- dependencyFlags is a bitmask of VkDependencyFlagBits.

*/
template <>
struct VkStructSetter<VkSubpassDependency,
                      SubpassDependencyFlags,
                      SubpassDependencyOpts> {

  static void set(VkSubpassDependency &dependency,
                  SubpassDependencyFlags &flags,
                  SubpassDependencyOpts &opts) {
    //
    dependency.srcSubpass = std::get<0>(flags);
    dependency.dstSubpass = std::get<1>(flags);
    dependency.srcStageMask = std::get<2>(flags);
    dependency.dstStageMask = std::get<3>(flags);
    dependency.srcAccessMask = std::get<4>(flags);
    dependency.dstAccessMask = std::get<5>(flags);

    auto flagRef = std::get<0>(opts);

    if (flagRef.has_value()) {
      auto fs = flagRef.value();
      VkDependencyFlags f = fs.obj()[0];
      unsigned int size =
          static_cast<unsigned int>(fs.length());
      for (unsigned int i = 1; i < size; i++) {
        f |= fs.obj()[i];
      }
      dependency.dependencyFlags = f;
    }
  }
};

constexpr void
VkFlagSetter(VkSubpassDependency &dependency,
             const std::uint32_t &srcSubpass,
             const std::uint32_t &dstSubpass,
             const VkPipelineStageFlags &srcStageMask,
             const VkPipelineStageFlags &dstStageMask,
             const VkAccessFlags &srcAccessMask,
             const VkAccessFlags &dstAccessMask) {
  //
  dependency.srcSubpass = srcSubpass;
  dependency.dstSubpass = dstSubpass;
  dependency.srcStageMask = srcStageMask;
  dependency.dstStageMask = dstStageMask;
  dependency.srcAccessMask = srcAccessMask;
  dependency.dstAccessMask = dstAccessMask;
}

void VkOptSetter(
    VkSubpassDependency &dependency,
    const std::optional<array_vk<VkDependencyFlagBits>>
        &flagRef = std::nullopt) {
  if (flagRef.has_value()) {
    auto fs = flagRef.value();
    VkDependencyFlags f = fs.obj()[0];
    unsigned int size =
        static_cast<unsigned int>(fs.length());
    for (unsigned int i = 1; i < size; i++) {
      f |= fs.obj()[i];
    }
    dependency.dependencyFlags = f;
  }
}

}; // namespace vtuto
