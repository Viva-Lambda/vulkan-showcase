#pragma once
#include <external.hpp>
#include <vkqueuefamily/queue.hpp>
#include <vkresult/vkoptional.hpp>

namespace vtuto {

/**
typedef enum VkQueueFlagBits {

    VK_QUEUE_GRAPHICS_BIT = 0x00000001,

    VK_QUEUE_COMPUTE_BIT = 0x00000002,

    VK_QUEUE_TRANSFER_BIT = 0x00000004,

    VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
      // Provided by VK_VERSION_1_1

    VK_QUEUE_PROTECTED_BIT = 0x00000010,
#ifdef VK_ENABLE_BETA_EXTENSIONS
      // Provided by VK_KHR_video_decode_queue

    VK_QUEUE_VIDEO_DECODE_BIT_KHR = 0x00000020,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      // Provided by VK_KHR_video_encode_queue

    VK_QUEUE_VIDEO_ENCODE_BIT_KHR = 0x00000040,
#endif
} VkQueueFlagBits;
 */
struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() &&
           presentFamily.has_value();
  }
  std::uint32_t nb_families() {
    // we have two families, present and graphics
    return 2;
  }
};

QueueFamilyIndices
findQueueFamilies(VkPhysicalDevice device,
                  VkSurfaceKHR surface) {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(
      queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
                                         &presentSupport);

    if (presentSupport) {
      indices.presentFamily = i;
    }

    if (indices.isComplete()) {
      break;
    }

    i++;
  }

  return indices;
}

} // namespace vtuto
