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

} // namespace vtuto
