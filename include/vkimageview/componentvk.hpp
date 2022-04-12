#pragma once

#include <external.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

// struct setter
#include <vkutils/varia.hpp>

using namespace vtuto;

namespace vtuto {
//
/**
typedef struct VkComponentMapping {
    VkComponentSwizzle r;
    VkComponentSwizzle g;
    VkComponentSwizzle b;
    VkComponentSwizzle a;
} VkComponentMapping;

- r is a VkComponentSwizzle specifying the component value
placed in the R
component of the output vector.

- g is a VkComponentSwizzle specifying the component value
placed in the G
component of the output vector.

- b is a VkComponentSwizzle specifying the component value
placed in the B
component of the output vector.

- a is a VkComponentSwizzle specifying the component value
placed in the A
component of the output vector.
 */
constexpr void
VkFlagSetter(VkImageViewCreateInfo &createInfo,
             const VkComponentSwizzle &r,
             const VkComponentSwizzle &g,
             const VkComponentSwizzle &b,
             const VkComponentSwizzle &a) {
  createInfo.components.r = r;
  createInfo.components.g = g;
  createInfo.components.b = b;
  createInfo.components.a = a;
}

} // namespace vtuto
