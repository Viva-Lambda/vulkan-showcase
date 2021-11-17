#pragma once
#include <external.hpp>

namespace vtuto {
//
std::vector<const char *> device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct QueuFamilyIndices {
  std::optional<uint32_t> graphics_family;
  std::optional<uint32_t> present_family;
  bool is_complete() {
    return graphics_family.has_value() && present_family.has_value();
  }
  /**
  Find device family indices for given VkPhysicalDevice

  We query the given physical device for physical device
  family properties. We break away if the device has
  complete
  number of indices for given device family.
  */

  static QueuFamilyIndices find_family_indices(VkPhysicalDevice pdev,
                                               VkSurfaceKHR surface) {
    QueuFamilyIndices indices;
    uint32_t familyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &familyCount, nullptr);
    //
    std::vector<VkQueueFamilyProperties> queueFamilies(familyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &familyCount,
                                             queueFamilies.data());

    uint32_t i = 0;
    for (const auto &qfamily : queueFamilies) {
      //
      if (qfamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.graphics_family = i;
      }

      VkBool32 present_support = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(pdev, i, surface, &present_support);

      if (present_support) {
        indices.present_family = i;
      }

      if (indices.is_complete()) {
        break;
      }
      i++;
    }
    return indices;
  }
};
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> present_modes;

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

  static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pdev,
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
};

typedef unsigned int uint;
} // namespace vtuto
