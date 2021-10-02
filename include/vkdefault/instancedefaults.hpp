// vk instance defaults
#pragma once
#include <external.hpp>
#include <initvk/vkInstance.hpp>
#include <vkdefault/debugdefaults.hpp>

namespace vtuto {

/**
  \brief get required extensions from glfw and debug utils
  First we query glfw api for enabled extensions. Then we
  add the debug flag
  to enable validation layers.
 */
std::vector<const char *> getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  // we reserve the size for the requested extensions
  // vector
  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);
  if (enableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  return extensions;
}

AppInfo_Vk_Params mkAppInfo() {
  //
  AppInfo_Vk_Params app;
  app.app_version[0] = 1;
  app.app_version[1] = 0;
  app.app_version[2] = 0;

  app.engine_version[0] = 1;
  app.engine_version[1] = 0;
  app.engine_version[2] = 0;

  app.app_name = "My Vulkan Application";
  app.engine_name = "Custom Engine";

  app.api_version = VK_API_VERSION_1_2;
  return app;
}

InstanceInfo_Vk_Params mkInstanceInfoParams() {

  InstanceInfo_Vk_Params params;
  auto app = mkAppInfo();
  //
  params.appInfo = app.mkApplicationInfo();
  params.extensions = getRequiredExtensions();
  std::vector<const char *> khronos_layers = {"VK_LAYER_KHRONOS_validation"};
  params.validation_layers = khronos_layers;
  params.debugCreateInfoExt = mkDebugCreateInfo();
  return params;
}

void mkInstance(VkInstance &instance) {

  InstanceInfo_Vk_Params params = mkInstanceInfoParams();

  VkInstanceCreateInfo createInfo = params.mkInstanceCreateInfo();
  Result_Vk res;
  CHECK_VK(vkCreateInstance(&createInfo, nullptr, &instance),
           "Failed to create Vulkan instance", res);
}

/**make a default instance*/
VkInstance mkInstance() {
  VkInstance instance;
  mkInstance(instance);
  return instance;
}

} // namespace vtuto
