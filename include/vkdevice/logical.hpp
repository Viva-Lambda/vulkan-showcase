#pragma once
// logical device properties and related code
#include <external.hpp>
#include <vkqueuefamily/index.hpp>
#include <vkqueuefamily/queue.hpp>
#include <vkutils/litutils.hpp>
#include <vkutils/varia.hpp>

// debug
#include <vkresult/debug.hpp>

namespace vtuto {
//
constexpr void
VkFlagSetter(VkDeviceCreateInfo &createInfo) {
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
}

void VkArgSetter(
    VkDeviceCreateInfo &createInfo,
    std::vector<VkDeviceQueueCreateInfo> &queueCreateInfos,
    VkPhysicalDeviceFeatures &deviceFeatures,
    const std::vector<const char *> &deviceExtensions) {
  //
  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames =
      deviceExtensions.data();

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames =
        validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }
}
}
