#pragma once
#include <external.hpp>
#include <vkqueuefamily/index.hpp>
#include <vkqueuefamily/queue.hpp>
#include <vkswapchain/support.hpp>

namespace vtuto {
//
/**
typedef struct VkSwapchainCreateInfoKHR {

    VkStructureType sType;

    const void* pNext;

    VkSwapchainCreateFlagsKHR flags;

    VkSurfaceKHR surface;

    uint32_t minImageCount;

    VkFormat imageFormat;

    VkColorSpaceKHR imageColorSpace;

    VkExtent2D imageExtent;

    uint32_t imageArrayLayers;

    VkImageUsageFlags imageUsage;

    VkSharingMode imageSharingMode;

    uint32_t queueFamilyIndexCount;

    const uint32_t* pQueueFamilyIndices;

    VkSurfaceTransformFlagBitsKHR preTransform;

    VkCompositeAlphaFlagBitsKHR compositeAlpha;

    VkPresentModeKHR presentMode;

    VkBool32 clipped;

    VkSwapchainKHR oldSwapchain;
} VkSwapchainCreateInfoKHR;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this structure.
- flags is a bitmask of VkSwapchainCreateFlagBitsKHR indicating parameters of
the swapchain creation.
- surface is the surface onto which the swapchain will present images. If the
creation succeeds, the swapchain becomes associated with surface.
- minImageCount is the minimum number of presentable images that the
application needs. The implementation will either create the swapchain with at
least that many images, or it will fail to create the swapchain.
- imageFormat is a VkFormat value specifying the format the swapchain image(s)
will be created with.
- imageColorSpace is a VkColorSpaceKHR value specifying the way the swapchain
interprets image data.
- imageExtent is the size (in pixels) of the swapchain image(s). The behavior
is platform-dependent if the image extent does not match the surface’s
currentExtent as returned by vkGetPhysicalDeviceSurfaceCapabilitiesKHR.


Here is the image usage flag bits:
// Provided by VK_VERSION_1_0
typedef enum VkImageUsageFlagBits {

VK_IMAGE_USAGE_TRANSFER_SRC_BIT = 0x00000001,

VK_IMAGE_USAGE_TRANSFER_DST_BIT = 0x00000002,

VK_IMAGE_USAGE_SAMPLED_BIT = 0x00000004,

VK_IMAGE_USAGE_STORAGE_BIT = 0x00000008,

VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT = 0x00000010,

VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT = 0x00000020,

VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT = 0x00000040,

VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT = 0x00000080,
#ifdef VK_ENABLE_BETA_EXTENSIONS
  // Provided by VK_KHR_video_decode_queue

VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR = 0x00000400,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
  // Provided by VK_KHR_video_decode_queue

VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR = 0x00000800,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
  // Provided by VK_KHR_video_decode_queue

VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR = 0x00001000,
#endif
  // Provided by VK_EXT_fragment_density_map

VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT = 0x00000200,
  // Provided by VK_KHR_fragment_shading_rate

VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR = 0x00000100,
#ifdef VK_ENABLE_BETA_EXTENSIONS
  // Provided by VK_KHR_video_encode_queue

VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR = 0x00002000,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
  // Provided by VK_KHR_video_encode_queue

VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR = 0x00004000,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
  // Provided by VK_KHR_video_encode_queue

VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR = 0x00008000,
#endif
  // Provided by VK_HUAWEI_invocation_mask

VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI = 0x00040000,
  // Provided by VK_NV_shading_rate_image

VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV =
VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
} VkImageUsageFlagBits;

- VK_IMAGE_USAGE_TRANSFER_SRC_BIT specifies that the image can be used as the
source of a transfer command.
- VK_IMAGE_USAGE_TRANSFER_DST_BIT specifies that the image can be used as the
destination of a transfer command.
- VK_IMAGE_USAGE_SAMPLED_BIT specifies that the image can be used to create a
VkImageView suitable for occupying a VkDescriptorSet slot either of type
VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
and be sampled by a shader.
- VK_IMAGE_USAGE_STORAGE_BIT specifies that the image can be used to create a
VkImageView suitable for occupying a VkDescriptorSet slot of type
VK_DESCRIPTOR_TYPE_STORAGE_IMAGE.
- VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT specifies that the image can be used to
create a VkImageView suitable for use as a color or resolve attachment in a
VkFramebuffer.
- VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT specifies that the image can be
used to create a VkImageView suitable for use as a depth/stencil or
depth/stencil resolve attachment in a VkFramebuffer.
- VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT specifies that the memory bound to
this image will be allocated with the VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT
(see Memory Allocation for more detail). This bit can be set for any image
that can be used to create a VkImageView suitable for use as a color, resolve,
depth/stencil, or input attachment.
- VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT specifies that the image can be used to
create a VkImageView suitable for occupying VkDescriptorSet slot of type
VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; be read from a shader as an input
attachment; and be used as an input attachment in a framebuffer.
- VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT specifies that the image can be
used to create a VkImageView suitable for use as a fragment density map image.
- VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR specifies that the
image can be used to create a VkImageView suitable for use as a fragment
shading rate attachment or shading rate image
- VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR specifies that video decode
operations can use the image as a reference picture, or a source of
postprocessing, or transfer operation.
- VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR specifies that video decode
operations can use the image as a DPB Video Picture Resource, representing a
reference picture. This flag must be combined with
VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR and
VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR.
VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR is required for DPB image resources
because these resources will be associated with reference pictures after
acting as targets for video decode operations.
VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR is required for DPB image resources
because the reference pictures associated with them act as information sources
for subsequent decode operations.
- VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR specifies that the image can be used
as an input picture for video encode operations.
- VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR specifies that video encode
operations can use the image to hold a reconstructed picture, and to act as a
reference picture. This flag must be combined with
VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR and
VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR.
VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR is required for a DPB image resource
because the reconstructed picture resulting from an encode operation will be
written to it. VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR is required for DPB
image

 */
template <
    VkFormat Format = VK_FORMAT_B8G8R8A8_SRGB,
    VkColorSpaceKHR ColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
    VkPresentModeKHR PresMode = VK_PRESENT_MODE_MAILBOX_KHR,
    VkImageUsageFlags ImufBits = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    VkCompositeAlphaFlagBitsKHR AlphaBits = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    VkBool32 IsClipped = VK_TRUE, VkQueueFlagBits... FBits>
void createSwapChainInfo(VkSwapchainCreateInfoKHR &createInfo,
                         VkPhysicalDevice &pdevice, VkSurfaceKHR &surface,
                         GLFWwindow *window, std::uint32_t &imageCount,
                         VkSurfaceFormatKHR &surfaceFormat,
                         VkExtent2D &extent) {

  SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport(pdevice, surface);

  surfaceFormat =
      chooseSwapSurfaceFormat<Format, ColorSpace>(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      chooseSwapPresentMode<PresMode>(swapChainSupport.present_modes);
  extent = chooseSwapExtent(swapChainSupport.capabilities, window);

  imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  //
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;

  // queried info from surface and physical device
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;

  //
  createInfo.imageUsage = ImufBits;
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.presentMode = presentMode;

  //
  createInfo.compositeAlpha = AlphaBits;
  createInfo.presentMode = presentMode;
  createInfo.clipped = IsClipped;
  //
  QueueFamilyIndices indices = find_queue_families<FBits...>(pdevice, surface);
  const auto queueFamilyIndices = indices.values();
  uint32_t graphicsFamily = 0;
  indices.index<FBits...>(graphicsFamily);

  if (graphicsFamily != indices.presentFamily.value()) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
    createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }
}

} // namespace vtuto
