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

template <VkQueueFlagBits... Fs> struct VkQueueFlagList {
  static constexpr std::array<VkQueueFlagBits, sizeof...(Fs)> flags = {{Fs...}};
};

typedef std::pair<std::optional<VkQueueFlagBits>, std::optional<uint32_t>>
    QueueFamily;

template <> struct VkOptional<QueueFamily> {
  static bool has_value(const QueueFamily &q) { return q.second.has_value(); }
};

typedef std::optional<uint32_t> PresentFamily;

template <> struct VkOptional<PresentFamily> {
  static bool has_value(const PresentFamily &q) { return q.has_value(); }
};

template <> struct VkOptional<queue_index_map> {
  static bool has_value(const queue_index_map &qfs) {
    auto checker = [](QueueFamily q) { return q.second.has_value(); };
    return std::all_of(qfs.begin(), qfs.end(), checker);
  }
};

struct QueueFamilyIndices {
  queue_index_map qfamilies;
  PresentFamily presentFamily;

  bool isComplete() {
    return VkOptional<queue_index_map>::has_value(qfamilies) &&
           VkOptional<PresentFamily>::has_value(presentFamily);
  }
  void set_qfamily(unsigned int i, VkQueueFlagBits flag) {
    qfamilies[flag] = i;
  }
  void set_pfamily(unsigned int i) { presentFamily = i; }
  std::vector<uint32_t> queue_values() const {
    //
    std::vector<uint32_t> qs;
    for (const auto &q : qfamilies) {
      if (q.second.has_value()) {
        qs.push_back(q.second.value());
      }
    }
    return qs;
  }
  std::vector<uint32_t> values() const {
    auto qs = queue_values();
    if (presentFamily.has_value()) {
      qs.push_back(presentFamily.value());
    }
    return qs;
  }
  template <VkQueueFlagBits f> void index(uint32_t &i) {
    if (qfamilies[f].has_value()) {
      i = qfamilies[f].value();
    }
  }
};

template <VkQueueFlagBits... Ts>
void queue_insert(VkQueueFamilyProperties queueFamily, QueueFamilyIndices &q,
                  unsigned int i) {
  for (const auto &obj : VkQueueFlagList<Ts...>::flags) {
    if (queueFamily.queueFlags & obj) {
      q.set_qfamily(i, obj);
    }
  }
}

template <VkQueueFlagBits... Fs>
QueueFamilyIndices find_queue_families(const VkPhysicalDevice &device,
                                       const VkSurfaceKHR &surface) {

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  QueueFamilyIndices indices;

  // query present family

  for (unsigned int i = 0; i < queueFamilies.size(); i++) {
    VkQueueFamilyProperties queueFamily = queueFamilies[i];

    queue_insert<Fs...>(queueFamily, indices, i);

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (presentSupport) {
      indices.set_pfamily(i);
    }

    if (indices.isComplete()) {
      break;
    }
  }
  return indices;
}
} // namespace vtuto
