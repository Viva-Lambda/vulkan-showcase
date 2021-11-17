#pragma once
// swapchain support
#include <external.hpp>

namespace vtuto {
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> present_modes;
};

/**
  Query supported swap chain details.

  \c vkGetPhysicalDeviceSurfaceFormatsKHR() \c for
  querying supported
  color formats (unorm, srgb, etc)

  \c vkGetPhysicalDeviceSurfacePresentModesKHR \c for
  querying supported
  data structures for presentation to be used in swap
  chain management.
 */

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pdev,
                                              VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pdev, surface,
                                            &details.capabilities);

  uint32_t format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(pdev, surface, &format_count, nullptr);

  if (format_count != 0) {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(pdev, surface, &format_count,
                                         details.formats.data());
  }
  uint32_t present_count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(pdev, surface, &present_count,
                                            nullptr);

  if (present_count != 0) {
    details.present_modes.resize(present_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(pdev, surface, &present_count,
                                              details.present_modes.data());
  }
  return details;
}

/**
  We choose the swap surface format based on given color space and the format

// Provided by VK_KHR_surface
typedef enum VkColorSpaceKHR {

VK_COLOR_SPACE_SRGB_NONLINEAR_KHR = 0,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT = 1000104001,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT = 1000104002,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT = 1000104003,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT = 1000104004,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_BT709_LINEAR_EXT = 1000104005,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_BT709_NONLINEAR_EXT = 1000104006,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_BT2020_LINEAR_EXT = 1000104007,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_HDR10_ST2084_EXT = 1000104008,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_DOLBYVISION_EXT = 1000104009,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_HDR10_HLG_EXT = 1000104010,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT = 1000104011,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT = 1000104012,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_PASS_THROUGH_EXT = 1000104013,
  // Provided by VK_EXT_swapchain_colorspace

VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT = 1000104014,
  // Provided by VK_AMD_display_native_hdr

VK_COLOR_SPACE_DISPLAY_NATIVE_AMD = 1000213000,

VK_COLORSPACE_SRGB_NONLINEAR_KHR = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,

VK_COLOR_SPACE_DCI_P3_LINEAR_EXT = VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT,
} VkColorSpaceKHR;

- VK_COLOR_SPACE_SRGB_NONLINEAR_KHR specifies support for the sRGB color space.
- VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT specifies support for the Display-P3
color space to be displayed using an sRGB-like EOTF (defined below).
- VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT specifies support for the extended
sRGB color space to be displayed using a linear EOTF.
- VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT specifies support for the
extended sRGB color space to be displayed using an sRGB EOTF.
- VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT specifies support for the Display-P3
color space to be displayed using a linear EOTF.
- VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT specifies support for the DCI-P3 color
space to be displayed using the DCI-P3 EOTF. Note that values in such an image
are interpreted as XYZ encoded color data by the presentation engine.
- VK_COLOR_SPACE_BT709_LINEAR_EXT specifies support for the BT709 color space to
be displayed using a linear EOTF.
- VK_COLOR_SPACE_BT709_NONLINEAR_EXT specifies support for the BT709 color space
to be displayed using the SMPTE 170M EOTF.
- VK_COLOR_SPACE_BT2020_LINEAR_EXT specifies support for the BT2020 color space
to be displayed using a linear EOTF.
- VK_COLOR_SPACE_HDR10_ST2084_EXT specifies support for the HDR10 (BT2020 color)
space to be displayed using the SMPTE ST2084 Perceptual Quantizer (PQ) EOTF.
- VK_COLOR_SPACE_DOLBYVISION_EXT specifies support for the Dolby Vision (BT2020
color space), proprietary encoding, to be displayed using the SMPTE ST2084 EOTF.
- VK_COLOR_SPACE_HDR10_HLG_EXT specifies support for the HDR10 (BT2020 color
space) to be displayed using the Hybrid Log Gamma (HLG) EOTF.
- VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT specifies support for the AdobeRGB color
space to be displayed using a linear EOTF.
- VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT specifies support for the AdobeRGB color
space to be displayed using the Gamma 2.2 EOTF.
- VK_COLOR_SPACE_PASS_THROUGH_EXT specifies that color components are used “
s is”. Thi intended to allow applications to supply data for color spaces not
described here.
- VK_COLOR_SPACE_DISPLAY_NATIVE_AMD specifies support for the display’s nati
e color space. Th matches the color space expectations of AMD’s FreeSyn
2 standard, for displays supporting it.

 */
template <VkFormat Format = VK_FORMAT_B8G8R8A8_SRGB,
          VkColorSpaceKHR ColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR>
VkSurfaceFormatKHR
chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats) {
  //
  for (const auto &availableFormat : formats) {
    if (availableFormat.format == Format &&
        availableFormat.colorSpace == ColorSpace) {
      return availableFormat;
    }
  }
  return formats[0];
}

/**
// Provided by VK_KHR_surface
typedef enum VkPresentModeKHR {

    VK_PRESENT_MODE_IMMEDIATE_KHR = 0,

    VK_PRESENT_MODE_MAILBOX_KHR = 1,

    VK_PRESENT_MODE_FIFO_KHR = 2,

    VK_PRESENT_MODE_FIFO_RELAXED_KHR = 3,
      // Provided by VK_KHR_shared_presentable_image

    VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR = 1000111000,
      // Provided by VK_KHR_shared_presentable_image

    VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR = 1000111001,
} VkPresentModeKHR;

- VK_PRESENT_MODE_IMMEDIATE_KHR specifies that the presentation engine does not
wait for a vertical blanking period to update the current image, meaning this
mode may result in visible tearing.  No internal queuing of presentation
requests is needed, as the requests are applied immediately.

- VK_PRESENT_MODE_MAILBOX_KHR specifies that the presentation engine waits for
the next vertical blanking period to update the current image. Tearing cannot
be observed. An internal single- entry queue is used to hold pending
presentation requests. If the queue is full when a new presentation request is
received, the new request replaces the existing entry, and any images
associated with the prior entry become available for re-use by the
application. One request is removed from the queue and processed during each
vertical blanking period in which the queue is non-empty.

- VK_PRESENT_MODE_FIFO_KHR specifies that the presentation engine waits for
the next vertical blanking period to update the current image. Tearing cannot
be observed. An internal queue is used to hold pending presentation requests.
New requests are appended to the end of the queue, and one request is removed
from the beginning of the queue and processed during each vertical blanking
period in which the queue is non-empty. This is the only value of presentMode
that is required to be supported.

- VK_PRESENT_MODE_FIFO_RELAXED_KHR specifies that the presentation engine
generally waits for the next vertical blanking period to update the current
image. If a vertical blanking period has already passed since the last update
of the current image then the presentation engine does not wait for another
vertical blanking period for the update, meaning this mode may result in
visible tearing in this case. This mode is useful for reducing visual stutter
with an application that will mostly present a new image before the next
vertical blanking period, but may occasionally be late, and present a new
image just after the next vertical blanking period. An internal queue is used
to hold pending presentation requests. New requests are appended to the end of
the queue, and one request is removed from the beginning of the queue and
processed during or after each vertical blanking period in which the queue is
non-empty.

- VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR
specifies that the presentation engine and application have concurrent access
to a single image, which is referred to as a shared presentable image. The
presentation engine is only required to update the current image after a new
presentation request is received. Therefore the application must make a
presentation request whenever an update is required. However, the presentation
engine may update the current image at any point, meaning this mode may result
in visible tearing.

- VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR specifies that the
presentation engine and application have concurrent access to a single image,
which is referred to as a shared presentable image. The presentation engine
periodically updates the current image on its regular refresh cycle. The
application is only required to make one initial presentation request, after
which the presentation engine must update the current image without any need
for further presentation requests. The application can indicate the image
contents have been updated by making a presentation request, but this does not
guarantee the timing of when it will be updated. This mode may result in
visible tearing if rendering to the image is not timed correctly.

 */
template <VkPresentModeKHR PMode = VK_PRESENT_MODE_MAILBOX_KHR>
VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == PMode) {
      return availablePresentMode;
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

/**
  Get the swap chain extension based on the surface capabilities.

 */
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                            GLFWwindow *window) {
  // requested extent is not extremely large
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    // requested extent is extremely large so fall back to window extent
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

} // namespace vtuto
