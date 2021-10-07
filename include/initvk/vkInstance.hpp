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
};
/**
  VkInstanceCreateInfo from Vulkan 1.2 API docs:
struct VkInstanceCreateInfo {
    VkStructureType             sType;
    const void*                 pNext;
    VkInstanceCreateFlags       flags;
    const VkApplicationInfo*    pApplicationInfo;
    uint32_t                    enabledLayerCount;
    const char* const*          ppEnabledLayerNames;
    uint32_t                    enabledExtensionCount;
    const char* const*          ppEnabledExtensionNames;
};

- sType is the type of this structure.

- pNext is NULL or a pointer to a structure extending this structure.

- flags is reserved for future use.

- pApplicationInfo is NULL or a pointer to a VkApplicationInfo structure. If
not NULL, this information helps implementations recognize behavior inherent
to classes of applications. VkApplicationInfo is defined in detail below.

- enabledLayerCount is the number of global layers to enable.

- ppEnabledLayerNames is a pointer to an array of enabledLayerCount
null-terminated UTF-8 strings containing the names of layers to enable for the
created instance. The layers are loaded in the order they are listed in this
array, with the first array element being the closest to the application, and
the last array element being the closest to the driver. See the Layers section
for further details.

- enabledExtensionCount is the number of global extensions to enable.

- ppEnabledExtensionNames is a pointer to an array of enabledExtensionCount
null-terminated UTF-8 strings containing the names of extensions to enable.
 */
struct InstanceInfo_Vk_Params {
private:
  VkStructureType _stype = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  void *_pNext = NULL;

public:
  VkApplicationInfo appInfo;
  std::vector<const char *> extensions;
  std::vector<const char *> validation_layers;
  const unsigned int flags = 0;

  // debug utils create info extension
  DebugUtilsCreateInfoExt debugCreateInfoExt;

  VkStructureType stype() const { return _stype; }
  void *pnext() const { return _pNext; }

  VkInstanceCreateInfo mkInstanceCreateInfo() const {
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = stype();
    //

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
      createInfo.pNext = pnext();
    }
    return createInfo;
  }
};

/**
Explicit Valid usage for VkInstanceCreateInfo:

- If the pNext chain of VkInstanceCreateInfo includes a
VkDebugReportCallbackCreateInfoEXT structure, the list of enabled extensions
in ppEnabledExtensionNames must contain VK_EXT_debug_report

- If the pNext chain of VkInstanceCreateInfo includes a
VkDebugUtilsMessengerCreateInfoEXT structure, the list of enabled extensions
in ppEnabledExtensionNames must contain VK_EXT_debug_utils

Implicit Valid usage for VkInstanceCreateInfo:
- sType must be VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO

- Each pNext member of any structure (including this one) in the pNext chain
must be either NULL or a pointer to a valid instance of
VkDebugReportCallbackCreateInfoEXT, VkDebugUtilsMessengerCreateInfoEXT,
VkValidationFeaturesEXT, or VkValidationFlagsEXT

- The sType value of each struct in the pNext chain must be unique, with the
exception of structures of type VkDebugUtilsMessengerCreateInfoEXT

- If pApplicationInfo is not NULL, pApplicationInfo must be a valid pointer to
a valid VkApplicationInfo structure

- If enabledLayerCount is not 0, ppEnabledLayerNames must be a valid pointer
to an array of enabledLayerCount null-terminated UTF-8 strings

- If enabledExtensionCount is not 0, ppEnabledExtensionNames must be a valid
pointer to an array of enabledExtensionCount null-terminated UTF-8 strings

 */
/*
bool check_stype_chain(const InstanceInfo_Vk_Params &params) {
  //
  std::vector<bool> stypes;
  auto messCheck = [](const void *p) {
    VkDebugUtilsMessengerCreateInfoEXT debugStruct =
dynamic_cast<VkDebugUtilsMessengerCreateInfoEXT>(p); if (debugStruct != nullptr)
{ return *debugStruct.sType ==
             VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    } else {
      return false;
    }
  };
  auto reportCheck = [](const void *p) {
    auto debugStruct = dynamic_cast<VkDebugReportCallbackCreateInfoEXT*>(p);
    if (debugStruct != nullptr) {
      return *debugStruct.sType ==
             VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    } else {
      return false;
    }
  };
  auto bothCheck = [](const void *p) {
    bool isReport = reportCheck(p);
    bool isMess = messCheck(p);
    if (isReport || isMess) {
      return true;
    } else {
      return false;
    }
  };
  void *pn = params.pNext;
  while (bothCheck(pn)) {
    pn = pn.pNext;
  }
}
*/
template <> struct StructChecker<InstanceInfo_Vk_Params> {
  static Result_Vk check(const InstanceInfo_Vk_Params &params) {
    Result_Vk vr;
    if (params.stype() != VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO) {
      vr.status = STRUCT_TYPE_ERROR_VK;
      return vr;
    }
    /*if (check_stype_chain(params)) {
      vr.status = STYPE_CHAIN_ERROR_VK;
      return vr;
    }
    */
    return vr;
  }
};

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
Result_Vk createInstance(const AppInfo_Vk_Params &vAppInfo,
                         InstanceInfo_Vk_Params vInstInfo,
                         VkInstance &instance) {

  // 1. Create Application info struct
  VkApplicationInfo appInfo{};

  // check parameters
  Result_Vk res;
  res = StructChecker<AppInfo_Vk_Params>::check(vAppInfo);
  if (res.status != SUCCESS_VK) {
    std::string msg = "Application Info Problem";
    UPDATE_RESULT_VK(res, msg);
    return res;
  }
  // make required object
  appInfo = vAppInfo.mkApplicationInfo();

  // 2. Pass info struct to instance info
  vInstInfo.appInfo = appInfo;

  // check parameters
  res = StructChecker<InstanceInfo_Vk_Params>::check(vInstInfo);
  if (res.status != SUCCESS_VK) {
    std::string msg = "Instance create info problem";
    UPDATE_RESULT_VK(res, msg);
    return res;
  }

  // make required object
  VkInstanceCreateInfo createInfo = vInstInfo.mkInstanceCreateInfo();
  std::string msg = "Failed to create Vulkan instance";
  CHECK_VK(vkCreateInstance(&createInfo, nullptr, &instance), msg, res);

  // create instance
  return res;
}

} // namespace vtuto
