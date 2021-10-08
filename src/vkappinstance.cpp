// main file
#include <hellotriangle.hpp>

// #include <vkdefault/instancedefaults.hpp>
using namespace vtuto;

namespace vtuto {
//
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

void HelloTriangle::createInstance() {
  // 1. Create Application info struct
  VkApplicationInfo appInfo{};

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // structure type
  appInfo.pApplicationName = "My Triangle"; // utf-8 string name for app
  appInfo.applicationVersion =
      VK_MAKE_VERSION(1, 0, 0);      // semver uint app version
  appInfo.pEngineName = "No Engine"; // utf-8 string name for used engine
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // unsigned int
  appInfo.apiVersion = VK_API_VERSION_1_2;          // uint32_t

  // 2. Pass info struct to instance info
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // struct type
  createInfo.pApplicationInfo = &appInfo; // VkApplicationInfo reference

  // 3. Request extensions from glfw in order to visualize
  // vulkan application instance
  auto extensions = getRequiredExtensions();

  //
  // pass number of enabled extensions
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  // std::vector<const char *> containing extensions
  createInfo.ppEnabledExtensionNames = extensions.data();

  // 4. create debug messenger handler
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(requested_validation_layers.size());
    createInfo.ppEnabledLayerNames = requested_validation_layers.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    //
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  // 5. create the instance with the given information
  //
  // \param createInfo: create info specifies the application extentions
  // its version, its engine, and the api version.

  // \param pAllocator, nullptr here. Normally it has to be of the following
  //
  // typedef struct VkAllocationCallbacks {
  // void* pUserData;
  // PFN_vkAllocationFunction pfnAllocation;
  // PFN_vkReallocationFunction pfnReallocation;
  // PFN_vkFreeFunction pfnFree;
  // PFN_vkInternalAllocationNotification pfnInternalAllocation;
  // PFN_vkInternalFreeNotification pfnInternalFree;
  // } VkAllocationCallbacks;

  // pUserData: value to be interpreted by the callbacks.
  // pfnAllocation: pointer to application defined memory allocation function.
  // s signature must be of the following type:
  // typedef void* (VKAPI_PTR *PFN_vkAllocationFunction)(
  // void* pUserData,
  // size_t size,
  // size_t alignment,
  // VkSystemAllocationScope allocationScope);

  // pUserData: data to be allocated defined by the user.
  // size requested allocation amount
  // alignment requested alignment amount in bytes must be a power of two
  // allocationScope a flag from VkSystemAllocationScope enum.

  CHECK_VK2(vkCreateInstance(&createInfo, nullptr, &instance),
            "Failed to create Vulkan instance");
} // namespace vtuto

// void HelloTriangle::createInstance() { mkInstance(instance); }

} // namespace vtuto
