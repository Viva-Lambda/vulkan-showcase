#pragma once
// device
#include <external.hpp>
#include <vkqueuefamily/index.hpp>
#include <vkqueuefamily/queue.hpp>
#include <vkswapchain/support.hpp>
#include <vkutils/litutils.hpp>

namespace vtuto {
//

std::vector<VkExtensionProperties>
get_physical_device_extensions(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());
  return availableExtensions;
}

struct extension_comp {
  bool operator()(const VkExtensionProperties &p1,
                  const VkExtensionProperties &p2) const {
    return p1.extensionName < p2.extensionName;
  }
};

template <std::size_t N> struct PhysicalDeviceExtensionQuery {
  std::array<VkExtensionProperties, N> extension_props;

  PhysicalDeviceExtensionQuery(const std::array<VkExtensionProperties, N> &exts)
      : extension_props(exts) {}

  /**
    Get available physical device extensions and check if queried extensions
    are among them.  If all the queries physical device extensions are among
    available extensions, the counter and the number of elements in the set
    must be the same
   */
  bool is_supported(VkPhysicalDevice device) {
    auto exts = get_physical_device_extensions(device);
    bool has_support = true;
    for (const auto &name : extension_props) {
      bool has_name = false;
      for (const auto &extension : exts) {
        // string conversion is needed for comparing const char* with const
        // char[]
        auto ext_name = std::string(extension.extensionName);
        auto ename = std::string(name.extensionName);
        if (ename == ext_name) {
          has_name = true;
        }
      }
      if (!has_name) {
        has_support = false;
      }
    }
    return has_support;
  }
  std::array<VkExtensionProperties, N> data() const { return extension_props; }
  std::array<std::vector<char const *>, N> cdata() const {
    std::array<std::vector<char const *>, N> arr;
    for (unsigned int i = 0; i < N; i++) {
      arr[i] = std::vector<char const *>{extension_props[i].extensionName};
    }
    return arr;
  }
  uint32_t size() { return static_cast<uint32_t>(extension_props.size()); }
};

/**
  Check whether the physical device has the extension support or not.

  The problem here is that extension name macros has the type
  const char [MAX_CHAR_LIMIT] or something like that and the

  VkDeviceCreateInfo field ppEnabledExtensionNames expects const
  char[MAX_CHAR_LIMIT] typed pointers. we should be using literal classes and
  not const char*
 */
bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
  std::array<VkExtensionProperties, 1> arr = {
      {VK_KHR_SWAPCHAIN_EXTENSION_NAME, 1}};
  PhysicalDeviceExtensionQuery pdev_ext_query(arr);
  return pdev_ext_query.is_supported(device);
}

//
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  QueueFamilyIndices indices =
      find_queue_families<VK_QUEUE_GRAPHICS_BIT>(device, surface);

  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        querySwapChainSupport(device, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.present_modes.empty();
  }

  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

} // namespace vtuto
