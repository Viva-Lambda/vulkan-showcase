#pragma once
#include <external.hpp>
#include <vkutils/varia.hpp>

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
  std::optional<uint32_t> graphics_family;
  std::optional<uint32_t> present_family;
  std::optional<uint32_t> compute_family;
  std::optional<uint32_t> transfer_bit;
  std::optional<uint32_t> sparse_binding;

  template <bool graphics, bool present, bool compute,
            bool transfer, bool sparse>
  bool isComplete() {
    bool counter = true;
    if (graphics) {
      counter &= graphics_family.has_value();
    }
    if (present == true) {
      counter &= present_family.has_value();
    }
    if (compute == true) {
      counter &= compute_family.has_value();
    }
    if (transfer == true) {
      counter &= transfer_bit.has_value();
    }
    if (sparse == true) {
      counter &= sparse_binding.has_value();
    }
    return counter;
  }
  std::uint32_t nb_families() {
    // we have two families, present and graphics
    std::uint32_t counter = 0;
    if (graphics_family.has_value()) {
      counter++;
    }
    if (present_family.has_value()) {
      counter++;
    }
    if (transfer_bit.has_value()) {
      counter++;
    }
    if (compute_family.has_value()) {
      counter++;
    }
    if (sparse_binding.has_value()) {
      counter++;
    }
    return counter;
  }
};

QueueFamilyIndices
findQueueFamilies(VkPhysicalDevice &device,
                  VkSurfaceKHR &surface) {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(
      queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  const bool graphic = true;
  const bool present = true;
  const bool transfer = false;
  const bool sparse = false;
  const bool compute = false;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics_family = i;
    } else if (queueFamily.queueFlags &
               VK_QUEUE_COMPUTE_BIT) {
      indices.compute_family = i;
    } else if (queueFamily.queueFlags &
               VK_QUEUE_TRANSFER_BIT) {
      indices.transfer_bit = i;
    } else if (queueFamily.queueFlags &
               VK_QUEUE_SPARSE_BINDING_BIT) {
      indices.sparse_binding = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
                                         &presentSupport);

    if (presentSupport) {
      indices.present_family = i;
    }
    if (indices.isComplete<graphic, present, compute,
                           transfer, sparse>()) {
      break;
    }

    i++;
  }

  return indices;
}

/**
typedef struct VkDeviceQueueCreateInfo {

VkStructureType sType;

const void* pNext;
VkDeviceQueueCreateFlags flags;

uint32_t queueFamilyIndex;

uint32_t queueCount;

const float* pQueuePriorities;
} VkDeviceQueueCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is a bitmask indicating behavior of the queue.
- queueFamilyIndex is an unsigned integer indicating the
index of the queue
family in which to create the queue on this device. This
index corresponds to
the index of an element of the pQueueFamilyProperties array
that was returned
by vkGetPhysicalDeviceQueueFamilyProperties.
- queueCount is an unsigned integer specifying the number of
queues to create
in the queue family indicated by queueFamilyIndex.
- pQueuePriorities is a pointer to an array of queueCount
normalized floating
point values, specifying priorities of work that will be
submitted to each
created queue. See Queue Priority for more information.

5.3.4. Queue Priority

Each queue is assigned a priority, as set in the
VkDeviceQueueCreateInfo
structures when creating the device. The priority of each
queue is a
normalized floating point value between 0.0 and 1.0, which
is then translated
to a discrete priority level by the implementation. Higher
values indicate a
higher priority, with 0.0 being the lowest priority and 1.0
being the highest.

Within the same device, queues with higher priority may be
allotted more
processing time than queues with lower priority. The
implementation makes no
guarantees with regards to ordering or scheduling among
queues with the same
priority, other than the constraints defined by any explicit
synchronization
primitives. The implementation makes no guarantees with
regards to queues
across different devices.

An implementation may allow a higher-priority queue to
starve a lower-priority
queue on the same VkDevice until the higher-priority queue
has no further
commands to execute. The relationship of queue priorities
must not cause
queues on one VkDevice to starve queues on another VkDevice.
No specific
guarantees are made about higher priority queues receiving
more processing
time or better quality of service than lower priority
queues.
 */

constexpr void
VkFlagSetter(VkDeviceQueueCreateInfo &createInfo) {
  createInfo.sType =
      VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
}
void VkOptSetter(
    VkDeviceQueueCreateInfo &createInfo,
    std::optional<array_vk<VkDeviceQueueCreateFlagBits>>
        &qflag_opt) {
  if (qflag_opt.has_value()) {
    auto qflags = qflag_opt.value();
    VkDeviceQueueCreateFlags f = qflags.obj()[0];
    for (unsigned int i = 1; i < qflags.length(); i++) {
      f |= qflags.obj()[i];
    }
    createInfo.flags = f;
  }
}
void VkArgSetter(VkDeviceQueueCreateInfo &createInfo,
                 std::uint32_t family_index,
                 const_floats &priorities) {
  std::uint32_t count =
      static_cast<std::uint32_t>(priorities.length());
  createInfo.queueFamilyIndex = family_index;
  createInfo.queueCount = count;
  createInfo.pQueuePriorities = priorities.obj();
}

} // namespace vtuto
