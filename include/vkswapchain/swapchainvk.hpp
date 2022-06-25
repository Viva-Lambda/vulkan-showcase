#pragma once

#include <external.hpp>
#include <vkqueuefamily/index.hpp>
#include <vkswapchain/support.hpp>
#include <vkutils/varia.hpp>

using namespace vtuto;

namespace vtuto {

/**
struct VkSwapchainCreateInfoKHR {

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
}
- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is a bitmask of VkSwapchainCreateFlagBitsKHR
indicating parameters of
the swapchain creation.
- surface is the surface onto which the swapchain will
present images. If the
creation succeeds, the swapchain becomes associated with
surface.
- minImageCount is the minimum number of presentable images
that the
application needs. The implementation will either create the
swapchain with at
least that many images, or it will fail to create the
swapchain.
- imageFormat is a VkFormat value specifying the format the
swapchain image(s)
will be created with.
- imageColorSpace is a VkColorSpaceKHR value specifying the
way the swapchain
interprets image data.
- imageExtent is the size (in pixels) of the swapchain
image(s). The behavior
is platform-dependent if the image extent does not match the
surface’s
currentExtent as returned by
vkGetPhysicalDeviceSurfaceCapabilitiesKHR.
- imageArrayLayers is the number of views in a
multiview/stereo surface. For
non-stereoscopic-3D applications, this value is 1.
- imageUsage is a bitmask of VkImageUsageFlagBits describing
the intended
usage of the (acquired) swapchain images.
- imageSharingMode is the sharing mode used for the image(s)
of the swapchain.
- queueFamilyIndexCount is the number of queue families
having access to the
image(s) of the swapchain when imageSharingMode is
VK_SHARING_MODE_CONCURRENT.
- pQueueFamilyIndices is a pointer to an array of queue
family indices having
access to the images(s) of the swapchain when
imageSharingMode is
VK_SHARING_MODE_CONCURRENT.
- preTransform is a VkSurfaceTransformFlagBitsKHR value
describing the
transform, relative to the presentation engine’s natural
orientation, applied
to the image content prior to presentation. If it does not
match the
currentTransform value returned by
vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
the presentation engine will transform the image
content as part of the presentation operation.
- compositeAlpha is a VkCompositeAlphaFlagBitsKHR value
indicating the alpha
compositing mode to use when this surface is composited
together with other
surfaces on certain window systems.
- presentMode is the presentation mode the swapchain will
use. A swapchain’s
present mode determines how incoming present requests will
be processed and
queued internally.
- clipped specifies whether the Vulkan implementation is
allowed to discard
rendering operations that affect regions of the surface that
are not visible.
 */
constexpr void
VkFlagSetter(VkSwapchainCreateInfoKHR &createInfo,
             const std::uint32_t &image_array_layers,
             const array_vk<VkImageUsageFlagBits> &imflags,
             VkCompositeAlphaFlagBitsKHR &composite_bits) {
  createInfo.sType =
      VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.imageArrayLayers = image_array_layers;
  // create bitmask from flagbits
  VkImageUsageFlags f = imflags.obj()[0];
  auto size = static_cast<unsigned int>(imflags.length());
  for (unsigned int i = 1; i < size; i++) {
    f |= imflags.obj()[i];
  }
  createInfo.imageUsage = f;

  // no bitmask for composite_bits see spec
  createInfo.compositeAlpha = composite_bits;
}

void VkArgSetter(VkSwapchainCreateInfoKHR &createInfo,
                 SwapChainSupportDetails &swapChainSupport,
                 VkSurfaceKHR &surface, GLFWwindow *window,
                 VkPhysicalDevice &pdevice) {
  //
  VkSurfaceFormatKHR surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      chooseSwapPresentMode(swapChainSupport.present_modes);
  VkExtent2D extent = chooseSwapExtent(
      swapChainSupport.capabilities, window);
  //
  std::uint32_t imageCount =
      swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount >
          swapChainSupport.capabilities.maxImageCount) {
    imageCount =
        swapChainSupport.capabilities.maxImageCount;
  }
  createInfo.surface = surface;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  QueueFamilyIndices indices =
      findQueueFamilies(pdevice, surface);
  uint32_t queueFamilyIndices[] = {
      indices.graphics_family.value(),
      indices.present_family.value()};

  /**
    If the presentation queue is different from the graphics
    queue we need to
    specify the sharing mode in the swap chain. The sharing
    mode determines
    modality of the ownership of the image in the swap
    chain.
    If the sharing mode is exclusive the image is owned by a
    single queue
    family at the time. The ownership must be explicitly
    transferred to
    another queue before using it in another queue family.
    Exclusive sharing
    offers the best performance, and it is the default
    option in most of
    the hardware, however if you are dealing with a specific
    hardware, a
    rendering station for example, then you might want to
    specify concurrency
    by default.

    The concurrent mode requires us to specify which queue
    families are going
    to share the image beforehand. You need to specify how
    many families are
    going to share the image and the indices in which the
    sharing would take
    place.

    In our code the QueueFamilyIndices object has two
    families: present and
    graphics. If at some point this object decides to
    include, for example,
    compute family or transfer family or some other family
    specified by a
    VkQueueFlagBit, then this number 2 needs to be adjusted
   */
  if (indices.graphics_family != indices.present_family) {
    createInfo.imageSharingMode =
        VK_SHARING_MODE_CONCURRENT;
    // we only distinguish two families graphics family and
    // present family
    createInfo.queueFamilyIndexCount =
        indices.nb_families();
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }
  createInfo.preTransform =
      swapChainSupport.capabilities.currentTransform;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
}
}
