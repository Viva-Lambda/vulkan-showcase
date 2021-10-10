#pragma once

#include <external.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

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

- r is a VkComponentSwizzle specifying the component value placed in the R
component of the output vector.

- g is a VkComponentSwizzle specifying the component value placed in the G
component of the output vector.

- b is a VkComponentSwizzle specifying the component value placed in the B
component of the output vector.

- a is a VkComponentSwizzle specifying the component value placed in the A
component of the output vector.
 */
struct ComponentMapping {
  //
  VkComponentSwizzle r;
  VkComponentSwizzle g;
  VkComponentSwizzle b;
  VkComponentSwizzle a;
};

bool check_cswizzle(const VkComponentSwizzle &c) {
  //
  if (c == VK_COMPONENT_SWIZZLE_IDENTITY) {
    return true;
  } else if (c == VK_COMPONENT_SWIZZLE_ZERO) {
    return true;
  } else if (c == VK_COMPONENT_SWIZZLE_ONE) {
    return true;
  } else if (c == VK_COMPONENT_SWIZZLE_R) {
    return true;
  } else if (c == VK_COMPONENT_SWIZZLE_G) {
    return true;
  } else if (c == VK_COMPONENT_SWIZZLE_B) {
    return true;
  } else if (c == VK_COMPONENT_SWIZZLE_A) {
    return true;
  } else {
    return false;
  }
}

template <> struct StructChecker<ComponentMapping> {
  static Result_Vk check(const ComponentMapping &cm) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    if (check_cswizzle(cm.r)) {
      vr.status = COMPONENT_SWIZZLE_ERROR_VK;
      vr.status_info = R"(
VUID-VkComponentMapping-r-parameter ::
r must be a valid VkComponentSwizzle value
)";
      return vr;
    }
    if (check_cswizzle(cm.g)) {
      vr.status = COMPONENT_SWIZZLE_ERROR_VK;
      vr.status_info = R"(
VUID-VkComponentMapping-g-parameter ::
g must be a valid VkComponentSwizzle value
)";
      return vr;
    }
    if (check_cswizzle(cm.b)) {
      vr.status = COMPONENT_SWIZZLE_ERROR_VK;
      vr.status_info = R"(
VUID-VkComponentMapping-b-parameter ::
b must be a valid VkComponentSwizzle value
)";
      return vr;
    }
    if (check_cswizzle(cm.a)) {
      vr.status = COMPONENT_SWIZZLE_ERROR_VK;
      vr.status_info = R"(
VUID-VkComponentMapping-a-parameter ::
a must be a valid VkComponentSwizzle value
)";
      return vr;
    }
    return vr;
  }
};

Result_Vk createComponentMapping(VkComponentMapping &vkcm,
                                 const ComponentMapping &cm) {
  Result_Vk vr = StructChecker<ComponentMapping>::check(cm);
  if (vr.status != SUCCESS_OP) {
    std::string strmsg = "createComponentMapping";
    vr.context = strmsg;
    return vr;
  }
  vkcm.r = cm.r;
  vkcm.g = cm.g;
  vkcm.b = cm.b;
  vkcm.a = cm.a;
  return vr;
}

} // namespace vtuto
