// logical device for vulkan instance
#pragma once

#include <external.hpp>
#include <pdevice.hpp>
#include <support.hpp>
#include <utils.hpp>

using namespace vtuto;

namespace vtuto {
//
template <> class vulkan_device<VkDevice> {
public:
  /** logical device handler */
  VkDevice ldevice;

  /** graphics queue */
  VkQueue graphics_queue;

  /** window surface queue*/
  VkQueue present_queue;

public:
  vulkan_device() {}
  vulkan_device(
      bool enableVLayers,
      const vulkan_device<VkPhysicalDevice> &physical_dev) {
    QueuFamilyIndices indices =
        QueuFamilyIndices::find_family_indices(
            physical_dev.pdevice, physical_dev.surface);

    /**
      VkDeviceQueueCreateInfo
      queueFamilyIndex
     */
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphics_family.value(),
        indices.present_family.value()};

    float queuePriority = 1.0f;
    for (uint32_t qfamily : uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType =
          VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = qfamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
    }

    //
    VkPhysicalDeviceFeatures deviceFeature{};
    deviceFeature.samplerAnisotropy = VK_TRUE;

    //
    VkDeviceCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeature;
    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(device_extensions.size());
    createInfo.ppEnabledExtensionNames =
        device_extensions.data();

    //
    if (enableVLayers) {
      createInfo.enabledLayerCount = static_cast<uint32_t>(
          requested_validation_layers.size());
      createInfo.ppEnabledLayerNames =
          requested_validation_layers.data();
    } else {
      createInfo.enabledLayerCount = 0;
    }
    CHECK_VK2(vkCreateDevice(physical_dev.pdevice,
                            &createInfo, nullptr, &ldevice),
             "failed to create a logical device given "
             "create info params");

    //
    vkGetDeviceQueue(ldevice,
                     indices.graphics_family.value(), 0,
                     &graphics_queue);
    vkGetDeviceQueue(ldevice,
                     indices.present_family.value(), 0,
                     &present_queue);
  }
  void destroy() { vkDestroyDevice(ldevice, nullptr); }
  VkDevice device() { return ldevice; }
};
}
