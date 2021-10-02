#pragma once
#include <external.hpp>
#include <vkdebug/debug.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {

/**
 */
struct AppInfo_Vk_Params {
  std::string app_name = "My Triangle";
  unsigned int app_version[3];
  std::string engine_name = "No Engine";
  unsigned int engine_version[3];
  uint32_t api_version = VK_API_VERSION_1_0;
  const VkStructureType stype = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  const void *pNext = NULL;

  /** Make application info struct with given parameters*/
  VkApplicationInfo mkApplicationInfo() const {
    VkApplicationInfo appInfo{};
    appInfo.sType = stype;
    appInfo.pApplicationName = app_name.c_str();
    /**semver uint app version*/
    unsigned int major = app_version[0];
    unsigned int minor = app_version[1];
    unsigned int alpha = app_version[2];
    appInfo.applicationVersion = VK_MAKE_VERSION(major, minor, alpha);
    appInfo.pEngineName = engine_name.c_str();
    /**semver uint engine version*/
    major = engine_version[0];
    minor = engine_version[1];
    alpha = engine_version[2];

    appInfo.engineVersion = VK_MAKE_VERSION(major, minor, alpha);
    appInfo.apiVersion = api_version;
    return appInfo;
  }
};

/**
  Check AppInfo_Vk_Params values as per vulkan specification
  1.2

  Specification mentions the following for valid usage:

  If apiVersion is not 0, then it MUST be greater than or
  equal to VK_API_VERSION_1_0
 */
template <> struct StructChecker<AppInfo_Vk_Params> {
  static Result_Vk check(const AppInfo_Vk_Params &params) {
    Result_Vk vr;
    // check api version. If it is not zero it must be
    // bigger than 1.0
    if (params.api_version != 0) {
      if (params.api_version < VK_API_VERSION_1_0) {
        vr.status = STRUCT_PARAM_ERROR_VK;
        return vr;
      }
    }
    if (params.stype != VK_STRUCTURE_TYPE_APPLICATION_INFO) {
      vr.status = STRUCT_TYPE_ERROR_VK;
      return vr;
    }
    vr.status = SUCCESS_VK;
    return vr;
  }
}

struct InstanceInfo_Vk_Params {
  const VkStructureType stype = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  VkApplicationInfo appInfo;
  std::vector<const char *> extensions;
  std::vector<const char *> validation_layers;
  const void *pNext;
  const unsigned int flags = 0;

  // debug utils create info extension
  DebugUtilsCreateInfoExt debugCreateInfoExt;

  VkInstanceCreateInfo mkInstanceCreateInfo() const {
    VkInstanceCreateInfo createInfo.sType = stype;

    createInfo.pApplicationInfo = &appInfo; /**VkApplicationInfo reference*/

    // 3. Request extensions from glfw in order to visualize
    // vulkan
    // application instance
    //
    /** pass number of enabled extensions*/
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    /** std::vector<const char *> containing extensions */
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers) {
      createInfo.enabledLayerCount =
          static_cast<uint32_t>(validation_layers.size());
      createInfo.ppEnabledLayerNames = validation_layers.data();

      // 4. create debug messenger handler
      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo =
          debugCreateInfoExt.mkDebugMessenger();

      createInfo.pNext =
          static_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&debugCreateInfo);
    } else {
      //
      createInfo.enabledLayerCount = 0;
      createInfo.pNext = nullptr;
    }
    return createInfo;
  }
};

template <> struct StructChecker<InstanceInfo_Vk_Params> {
  static Result_Vk check(const InstanceInfo_Vk_Params &params) {
    Result_Vk vr = StructChecker<AppInfo_Vk_Params>::check(params.appInfo);
    if (vr.status != SUCCESS_VK)
      return vr;
    if (params.stype != VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO) {
      vr.status = STRUCT_TYPE_ERROR_VK;
    }
    return vr;
  }
}

/**
  Create a Vulkan Instance

  Creation has two steps:

  - Optional step: Application info

  - Required step: Instance info

  Application info contains regular
  information with respect to vulkan application.
  For example, name, version, whether it uses a
  specific engine etc. The information mentioned
  here can be used by the driver to optimize certain
  aspects of the runtime with respect to parameters
  entered here.

  The second info, that of instance, is gives information
  with respect to extensions, and validation layers
  we would like to use for the application instance.
 */
Result_Vk
createInstance(const AppInfo_Vk_Params &vAppInfo,
               InstanceInfo_Vk_Params vInstInfo, VkInstance &instance) {

  // 1. Create Application info struct
  VkApplicationInfo appInfo{};

  // check parameters
  Result_Vk res = StructChecker<AppInfo_Vk_Params>::check(vAppInfo);
  if (res.status != SUCCESS_VK) {
    UPDATE_RESULT_VK(res);
    return res;
  }
  // make required object
  VkApplicationInfo appInfo = vAppInfo.mkApplicationInfo();

  // 2. Pass info struct to instance info
  vInstInfo.appInfo = appInfo;

  // check parameters
  res = StructChecker<InstanceInfo_Vk_Params>::check(vInstInfo);
  if (res.status != SUCCESS_VK) {
    UPDATE_RESULT_VK(res);
    return res;
  }

  // make required object
  VkInstanceCreateInfo createInfo = vInstInfo.mkInstanceCreateInfo();
  CHECK_VK(vkCreateInstance(&createInfo, nullptr, &instance),
           "Failed to create Vulkan instance", res);

  // create instance
  return res;
}

} // namespace vtuto
