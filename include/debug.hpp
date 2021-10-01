#pragma once
#include <external.hpp>

/**
  standard validation functions are defined in
  VK_LAYER_KHRONOS_validation by
  the lunar sdk
 */
std::vector<const char *> requested_validation_layers = {
    "VK_LAYER_KHRONOS_validation"};

/**
  Enable validation layers for debug build. NDEBUG macro is
  a part of c++ standard.
 */

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

/**
  Create debug messenger.
*/
VkResult CreateDebugUtilsMessengerExt(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  //
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
      vkGetInstanceProcAddr(
          instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator,
                pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator) {
  //
  auto fn = (PFN_vkDestroyDebugUtilsMessengerEXT)
      vkGetInstanceProcAddr(
          instance, "vkDestroyDebugUtilsMessengerEXT");
  if (fn != nullptr) {
    fn(instance, debugMessenger, pAllocator);
  }
}
