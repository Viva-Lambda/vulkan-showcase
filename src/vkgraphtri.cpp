#include <external.hpp>
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vkgraphmaker.hpp>
#include <vkgraph/vknode.hpp>

using namespace vtuto;

static void framebufferResizeCallback(GLFWwindow *window, int width,
                                      int height) {
  auto app = reinterpret_cast<vk_graph *>(glfwGetWindowUserPointer(window));
  app->framebuffer_resized = true;
}
const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

static VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}
static bool checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

static void
DestroyDebugUtilsMessengerEXT(VkInstance instance,
                              VkDebugUtilsMessengerEXT debugMessenger,
                              const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

static std::vector<const char *> getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  if (enableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}
static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

static void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
}

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                            VkSurfaceKHR surface) {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (presentSupport) {
      indices.presentFamily = i;
    }

    if (indices.isComplete()) {
      break;
    }

    i++;
  }

  return indices;
}
static bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                           deviceExtensions.end());

  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}
static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                                     VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  QueueFamilyIndices indices = findQueueFamilies(device, surface);

  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        querySwapChainSupport(device, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

int main() {
  // declare graph
  vk_graph graph;
  unsigned int node_counter = 1;

  /** init window node */
  // declare node
  vk_node init_w_node;
  {
    init_w_node.is_singular = true;
    init_w_node.is_called = false;
    init_w_node.compute = [](vk_graph &myg) {
      glfwInit();

      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

      myg.window = glfwCreateWindow(myg.win_width, myg.win_height, "Vulkan",
                                    nullptr, nullptr);
      glfwSetWindowUserPointer(myg.window, &myg);
      glfwSetFramebufferSizeCallback(myg.window, framebufferResizeCallback);
      return;
    };
    init_w_node.node_id = node_counter;
    //
    node_counter++;
  }

  /** init vulkan node:
   It can be implemented either as a single node or a chain of sub nodes.
   I implement it as a sequence of nodes whose execution order is fixed by its
   edges. The node creation can be delegated to other encapsulating functions
   for facilitating reading
   */
  /** vulkan instance creation node*/
  vk_node createInstanceNode;
  {
    createInstanceNode.is_singular = true;
    createInstanceNode.is_called = false;
    createInstanceNode.compute = [](vk_graph &myg) {
      if (enableValidationLayers && !checkValidationLayerSupport()) {
        std::cerr << "validation layers requested, but not available!"
                  << std::endl;
      }
      VkApplicationInfo appInfo{};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "Hello Triangle";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "No Engine";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      auto extensions = getRequiredExtensions();
      createInfo.enabledExtensionCount =
          static_cast<uint32_t>(extensions.size());
      createInfo.ppEnabledExtensionNames = extensions.data();

      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
      if (enableValidationLayers) {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext =
            (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
      } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
      }

      if (vkCreateInstance(&createInfo, nullptr, &myg.instance) != VK_SUCCESS) {
        std::cerr << "instance creation failed" << std::endl;
      }
    };
    createInstanceNode.node_id = node_counter;

    // mkVkGNode(node_counter, create_inst_f, createInstanceNode);
    node_counter++;
  }

  /** vulkan debug messenger node*/
  vk_node setupDebugMessengerNode;
  {
    setupDebugMessengerNode.is_singular = true;
    setupDebugMessengerNode.is_called = false;
    setupDebugMessengerNode.compute = [](vk_graph &myg) {
      if (!enableValidationLayers)
        return;

      VkDebugUtilsMessengerCreateInfoEXT createInfo;
      populateDebugMessengerCreateInfo(createInfo);

      if (CreateDebugUtilsMessengerEXT(myg.instance, &createInfo, nullptr,
                                       &myg.debugMessenger) != VK_SUCCESS) {
        std::cerr << "failed to set up debug messenger!" << std::endl;
      }
    };
    setupDebugMessengerNode.node_id = node_counter;
    node_counter++;
  }

  /** vulkan create surface node*/
  vk_node createSurfaceNode;
  {
    createSurfaceNode.is_singular = true;
    createSurfaceNode.is_called = false;
    createSurfaceNode.compute = [](vk_graph &myg) {
      if (glfwCreateWindowSurface(myg.instance, myg.window, nullptr,
                                  &myg.surface) != VK_SUCCESS) {
        std::cerr << "failed to create window surface!" << std::endl;
      }
    };
    createSurfaceNode.node_id = node_counter;
    node_counter++;
  }

  /** vulkan pick physical device node*/
  vk_node pickPhysicalDeviceNode;
  {
    pickPhysicalDeviceNode.is_singular = true;
    pickPhysicalDeviceNode.is_called = false;
    pickPhysicalDeviceNode.compute = [](vk_graph &myg) {
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(myg.instance, &deviceCount, nullptr);

      if (deviceCount == 0) {
        std::cerr << "failed to find GPUs with Vulkan support!" << std::endl;
        return;
      }

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(myg.instance, &deviceCount, devices.data());

      for (const auto &device : devices) {
        if (isDeviceSuitable(device, myg.surface)) {
          myg.pdevice = device;
          break;
        }
      }

      if (myg.pdevice == VK_NULL_HANDLE) {
        std::cerr << "failed to find a suitable GPU!" << std::endl;
        return;
      }
    };
    pickPhysicalDeviceNode.node_id = node_counter;
    node_counter++;
  }

  // edges of the graph
  unsigned int edge_counter = 1;

  mkAddVkGEdge(graph, init_w_node, createInstanceNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createInstanceNode, setupDebugMessengerNode,
               edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createInstanceNode, createSurfaceNode, edge_counter);
  edge_counter++;

  // run first edge
  std::vector<vk_edge> ops(graph.ops.begin(), graph.ops.end());
  for (auto e : ops) {
    e.start.run(graph);
    e.end.run(graph);
  }

  std::cout << "everything runs" << std::endl;

  // start declaring nodes
  return 0;
}
