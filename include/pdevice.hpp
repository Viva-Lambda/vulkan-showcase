#pragma once
/**\brief Physical Device Object*/

#include <debug.hpp>
#include <device.hpp>
#include <external.hpp>
#include <support.hpp>
#include <utils.hpp>

using namespace vtuto;

namespace vtuto {

template <> class vulkan_device<VkPhysicalDevice> {
public:
  VkPhysicalDevice pdevice = VK_NULL_HANDLE;
  VkSurfaceKHR surface;
  VkInstance *instance_ptr;

public:
  vulkan_device() : instance_ptr(nullptr) {}

  VkPhysicalDevice device() { return pdevice; }
  void destroy() {
    vkDestroySurfaceKHR(instance(), surface, nullptr);
  }
  vulkan_device(VkInstance *ins, GLFWwindow *window)
      : instance_ptr(ins) {
    // 1. create surface
    createSurface(window);

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance(), &device_count,
                               nullptr);
    //
    if (device_count == 0) {
      throw std::runtime_error(
          "Vulkan api is not supported by your hardware");
    }
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance(), &device_count,
                               devices.data());

    for (const auto &device : devices) {
      //
      if (is_device_suitable(device)) {
        //
        pdevice = device;
        break;
      }
    }
    if (pdevice == VK_NULL_HANDLE) {
      //
      throw std::runtime_error("Your device does not "
                               "respond to any of "
                               "available queueFamilies");
    }
  }
  /**
      Check if the device is suitable for implementing a
     swap
      chain
     */

  bool is_device_suitable(VkPhysicalDevice pdev) {
    QueuFamilyIndices indices =
        QueuFamilyIndices::find_family_indices(pdev,
                                               surface);
    bool areExtensionsSupported =
        checkDeviceExtensionSupport(pdev);

    bool isSwapChainPossible = false;
    if (areExtensionsSupported) {
      SwapChainSupportDetails swapChainSupport =
          SwapChainSupportDetails::querySwapChainSupport(
              pdev, surface);

      isSwapChainPossible =
          !swapChainSupport.formats.empty() &&
          !swapChainSupport.present_modes.empty();
    }
    VkPhysicalDeviceProperties dprops{};
    vkGetPhysicalDeviceProperties(pdev, &dprops);
    bool cond1 = indices.is_complete() &&
           areExtensionsSupported && isSwapChainPossible;
    bool cond2 = cond1 && (dprops.limits.maxSamplerAnisotropy > 0.0);
    return cond2;
  }
  void createSurface(GLFWwindow *window) {
    CHECK_VK(glfwCreateWindowSurface(instance(), window,
                                     nullptr, &surface),
             "failed to create window surface");
  }
  VkInstance instance() { return *instance_ptr; }
  bool checkDeviceExtensionSupport(VkPhysicalDevice pdev) {
    //
    uint32_t ext_count = 0;
    vkEnumerateDeviceExtensionProperties(
        pdev, nullptr, &ext_count, nullptr);

    std::vector<VkExtensionProperties> available_exts(
        ext_count);

    vkEnumerateDeviceExtensionProperties(
        pdev, nullptr, &ext_count, available_exts.data());

    std::set<std::string> requested_extensions(
        device_extensions.begin(), device_extensions.end());

    for (const auto &ext : available_exts) {
      requested_extensions.erase(ext.extensionName);
    }
    return requested_extensions.empty();
  }
};
}
