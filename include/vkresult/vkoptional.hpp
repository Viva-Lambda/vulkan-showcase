#pragma once
// template for optional values in vulkan

namespace vtuto {
template <typename T> struct VkOptional {
  static bool has_value(const T &t) { return false; }
};
} // namespace vtuto
