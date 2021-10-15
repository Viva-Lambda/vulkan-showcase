#include <external.hpp>
#include <vkapp/vktriapp.hpp>
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vkgraphmaker.hpp>
#include <vkgraph/vknode.hpp>
#include <vkresult/debug.hpp>

using namespace vtuto;

static void framebufferResizeCallback(GLFWwindow *window, int width,
                                      int height) {
  auto app = reinterpret_cast<vk_triapp *>(glfwGetWindowUserPointer(window));
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
static VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

static VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                                   GLFWwindow *window) {
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}
static std::vector<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "failed to open file! " << filename << std::endl;
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}
static VkShaderModule createShaderModule(const std::vector<char> &code,
                                         VkDevice device) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) !=
      VK_SUCCESS) {
    std::cerr << "failed to create shader module!" << std::endl;
  }

  return shaderModule;
}

static const int MAX_FRAMES_IN_FLIGHT = 2;

static void drawFrame(vk_triapp &g) {
  vkWaitForFences(g.ldevice, 1, &g.current_fences[g.current_frame], VK_TRUE,
                  UINT64_MAX);

  uint32_t imageIndex;
  VkResult result =
      vkAcquireNextImageKHR(g.ldevice, g.chain, UINT64_MAX,
                            g.image_available_semaphores[g.current_frame],
                            VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    std::cerr << "failed to acquire swap chain image!" << std::endl;
    return;
  }

  if (g.images_in_flight[imageIndex] != VK_NULL_HANDLE) {
    vkWaitForFences(device, 1, &g.images_in_flight[imageIndex], VK_TRUE,
                    UINT64_MAX);
  }
  g.images_in_flight[imageIndex] = g.current_fences[g.current_frame];

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {
      g.image_available_semaphores[g.current_frame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &g.cbuffers[imageIndex];

  VkSemaphore signalSemaphores[] = {
      g.render_finished_semaphores[g.current_frame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  vkResetFences(g.ldevice, 1, &g.current_fences[g.current_frame]);

  if (vkQueueSubmit(g.graphics_queue, 1, &submitInfo,
                    g.current_fences[g.current_frame]) != VK_SUCCESS) {
    std::cerr << "failed to submit draw command buffer!" << std::endl;
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {g.chain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;

  presentInfo.pImageIndices = &imageIndex;

  result = vkQueuePresentKHR(presentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      g.framebuffer_resized) {
    g.framebuffer_resized = false;
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    std::cerr << "failed to present swap chain image!" << std::endl;
    return;
  }

  g.current_frame = (g.current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

int main() {
  // declare graph
  vk_graph<vk_triapp> graph;
  unsigned int node_counter = 1;

  /** init window node:
    - node id 1
    - target nodes: {2}
   */
  // declare node
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      glfwInit();

      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

      myg.window = glfwCreateWindow(myg.win_width, myg.win_height, "Vulkan",
                                    nullptr, nullptr);
      glfwSetWindowUserPointer(myg.window, &myg);
      glfwSetFramebufferSizeCallback(myg.window, framebufferResizeCallback);
      vk_output out;
      out.next_node = 2;
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      out.result_info = vr;
      return out;
    };
    auto vr = mkAddNode<vk_triapp, 1, true, 2>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** init vulkan node:
   It can be implemented either as a single node or a chain of sub nodes.
   I implement it as a sequence of nodes whose execution order is fixed by its
   edges. The node creation can be delegated to other encapsulating functions
   for facilitating reading
   */
  /** vulkan instance creation node:
    - node id 2
    - target nodes: {3}
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      //
      if (enableValidationLayers && !checkValidationLayerSupport()) {
        vr.context = "validation layers requested, but not available!";
        out.next_node = 0;
        vr.status = FAIL_OP;
        out.result_info = vr;
        return out;
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

      std::string nmsg = "instance creation failed";
      CHECK_VK(vkCreateInstance(&createInfo, nullptr, &myg.instance), nmsg, vr);
      out.result_info = vr;
      out.next_node = 3;
      return out;
    };
    auto vr = mkAddNode<vk_triapp, 2, true, 3>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 2 with end node 3";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan setup debug messenger node:
    - node id 3
    - target nodes: {4}
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      out.result_info = vr;
      out.next_node = 4;

      if (!enableValidationLayers) {
        out.next_node = 4;
        return out;
      }

      VkDebugUtilsMessengerCreateInfoEXT createInfo;
      populateDebugMessengerCreateInfo(createInfo);

      std::string nmsg = "failed to set up debug messenger!";
      CHECK_VK(CreateDebugUtilsMessengerEXT(myg.instance, &createInfo, nullptr,
                                            &myg.debugMessenger),
               nmsg, out.result_info);
      if (out.result_info.status != SUCCESS_OP) {
        out.next_node = 0;
        return out;
      } else {
        return out;
      }
    };
    auto vr = mkAddNode<vk_triapp, 3, true, 4>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 3 with end node 4";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan create surface node:
    - node id 4
    - target nodes: {5}
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      out.result_info = vr;
      out.next_node = 5;
      std::string nmsg = "failed to create window surface!";

      CHECK_VK(glfwCreateWindowSurface(myg.instance, myg.window, nullptr,
                                       &myg.surface),
               nmsg, out.result_info);
      if (out.result_info.status != SUCCESS_OP) {
        out.next_node = 0;
        return out;
      } else {
        return out;
      }
    };
    auto vr = mkAddNode<vk_triapp, 4, true, 5>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 4 with end node 5";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan pick physical device node:
    - node id 5
    - target nodes: {6}
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      out.result_info = vr;
      out.next_node = 6;

      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(myg.instance, &deviceCount, nullptr);

      if (deviceCount == 0) {
        out.result_info.status = FAIL_OP;
        out.result_info.context = "failed to find GPUs with Vulkan support!";
        out.next_node = 0;
        return out;
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
        out.result_info.status = FAIL_OP;
        out.result_info.context = "failed to find a suitable GPU!";
        out.next_node = 0;
        return out;
      }
      return out;
    };
    auto vr = mkAddNode<vk_triapp, 5, true, 6>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 5 with end node 6";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan create logical device node:
    - node id 6
    - target nodes: {7}
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      //
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      out.result_info = vr;
      out.next_node = 7;

      QueueFamilyIndices indices = findQueueFamilies(myg.pdevice, myg.surface);

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
      std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                                indices.presentFamily.value()};

      float queuePriority = 1.0f;
      for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
      }

      VkPhysicalDeviceFeatures deviceFeatures{};

      VkDeviceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      createInfo.queueCreateInfoCount =
          static_cast<uint32_t>(queueCreateInfos.size());
      createInfo.pQueueCreateInfos = queueCreateInfos.data();

      createInfo.pEnabledFeatures = &deviceFeatures;

      createInfo.enabledExtensionCount =
          static_cast<uint32_t>(deviceExtensions.size());
      createInfo.ppEnabledExtensionNames = deviceExtensions.data();

      if (enableValidationLayers) {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
      } else {
        createInfo.enabledLayerCount = 0;
      }

      std::string nmsg = "failed to create logical device!";

      CHECK_VK(vkCreateDevice(myg.pdevice, &createInfo, nullptr, &myg.ldevice),
               nmsg, out.result_info);
      if (out.result_info.status != SUCCESS_OP) {
        out.next_node = 0;
        return out;
      }

      vkGetDeviceQueue(myg.ldevice, indices.graphicsFamily.value(), 0,
                       &myg.graphics_queue);
      vkGetDeviceQueue(myg.ldevice, indices.presentFamily.value(), 0,
                       &myg.present_queue);
      return out;
    };
    auto vr = mkAddNode<vk_triapp, 6, true, 7>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 6 with end node 7";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan create swap chain node:
    - node id 7
    - target nodes: {8}
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      //
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      out.result_info = vr;
      out.next_node = 8;

      SwapChainSupportDetails swapChainSupport =
          querySwapChainSupport(myg.pdevice, myg.surface);

      VkSurfaceFormatKHR surfaceFormat =
          chooseSwapSurfaceFormat(swapChainSupport.formats);
      VkPresentModeKHR presentMode =
          chooseSwapPresentMode(swapChainSupport.presentModes);
      VkExtent2D extent =
          chooseSwapExtent(swapChainSupport.capabilities, myg.window);

      uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
      if (swapChainSupport.capabilities.maxImageCount > 0 &&
          imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
      }

      VkSwapchainCreateInfoKHR createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      createInfo.surface = myg.surface;

      createInfo.minImageCount = imageCount;
      createInfo.imageFormat = surfaceFormat.format;
      createInfo.imageColorSpace = surfaceFormat.colorSpace;
      createInfo.imageExtent = extent;
      createInfo.imageArrayLayers = 1;
      createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      QueueFamilyIndices indices = findQueueFamilies(myg.pdevice, myg.surface);
      uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                       indices.presentFamily.value()};

      if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
      } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      }

      createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
      createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      createInfo.presentMode = presentMode;
      createInfo.clipped = VK_TRUE;

      std::string nmsg = "failed to create swap chain!";
      CHECK_VK(
          vkCreateSwapchainKHR(myg.ldevice, &createInfo, nullptr, &myg.chain),
          nmsg, out.result_info);

      if (out.result_info.status != SUCCESS_OP) {
        out.next_node = 0;
        return out;
      }

      vkGetSwapchainImagesKHR(myg.ldevice, myg.chain, &imageCount, nullptr);
      myg.simages.resize(imageCount);
      vkGetSwapchainImagesKHR(myg.ldevice, myg.chain, &imageCount,
                              myg.simages.data());

      myg.simage_format = surfaceFormat.format;
      myg.sextent = extent;
      return out;
    };
    auto vr = mkAddNode<vk_triapp, 7, false, 8>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 7 with end node 8";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create swap chain image views node:
    - node id 8
    - target nodes {9}
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      out.result_info = vr;
      out.next_node = 9;

      myg.views.resize(myg.simages.size());

      for (size_t i = 0; i < myg.simages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = myg.simages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = myg.simage_format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        std::string nmsg = "failed to create image views!";
        CHECK_VK(
            vkCreateImageView(myg.ldevice, &createInfo, nullptr, &myg.views[i]),
            nmsg, out.result_info);

        if (out.result_info.status != SUCCESS_OP) {
          out.next_node = 0;
          return out;
        }
      }
      return out;
    };
    auto vr = mkAddNode<vk_triapp, 8, false, 9>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 8 with end node 9";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create render pass for all the images node:
    - node id 9
    - target nodes {10}
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      //
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      out.result_info = vr;
      out.next_node = 10;

      VkAttachmentDescription colorAttachment{};
      colorAttachment.format = myg.simage_format;
      colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      VkAttachmentReference colorAttachmentRef{};
      colorAttachmentRef.attachment = 0;
      colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      VkSubpassDescription subpass{};
      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = &colorAttachmentRef;

      VkSubpassDependency dependency{};
      dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
      dependency.dstSubpass = 0;
      dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.srcAccessMask = 0;
      dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

      VkRenderPassCreateInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount = 1;
      renderPassInfo.pAttachments = &colorAttachment;
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subpass;
      renderPassInfo.dependencyCount = 1;
      renderPassInfo.pDependencies = &dependency;

      std::string nmsg = "failed to create render pass!";

      CHECK_VK(vkCreateRenderPass(myg.ldevice, &renderPassInfo, nullptr,
                                  &myg.render_pass),
               nmsg, out.result_info);
      if (out.result_info.status != SUCCESS_OP) {
        out.next_node = 0;
        return out;
      } else {
        return out;
      }
    };
    auto vr = mkAddNode<vk_triapp, 9, false, 10>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 9 with end node 10";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create graphics pipeline node:
    - node id 10
    - target nodes {11}

    It accesses IO. It can probably be refactored to access it only one time.
   */
  {
    std::function<vk_output(vk_triapp &)> f = [](vk_triapp &myg) {
      Result_Vk vr;
      vr.status = SUCCESS_OP;
      vk_output out;
      out.result_info = vr;
      out.next_node = 11;

      //
      auto vertShaderCode = readFile("shaders/triangle/triangle.vert.spv");
      auto fragShaderCode = readFile("shaders/triangle/triangle.frag.spv");

      VkShaderModule vertShaderModule =
          createShaderModule(vertShaderCode, myg.ldevice);
      VkShaderModule fragShaderModule =
          createShaderModule(fragShaderCode, myg.ldevice);

      VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
      vertShaderStageInfo.sType =
          VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
      vertShaderStageInfo.module = vertShaderModule;
      vertShaderStageInfo.pName = "main";

      VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
      fragShaderStageInfo.sType =
          VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      fragShaderStageInfo.module = fragShaderModule;
      fragShaderStageInfo.pName = "main";

      VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                        fragShaderStageInfo};

      VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
      vertexInputInfo.sType =
          VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      vertexInputInfo.vertexBindingDescriptionCount = 0;
      vertexInputInfo.vertexAttributeDescriptionCount = 0;

      VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
      inputAssembly.sType =
          VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
      inputAssembly.primitiveRestartEnable = VK_FALSE;

      VkViewport viewport{};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = (float)myg.sextent.width;
      viewport.height = (float)myg.sextent.height;
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;

      VkRect2D scissor{};
      scissor.offset = {0, 0};
      scissor.extent = myg.sextent;

      VkPipelineViewportStateCreateInfo viewportState{};
      viewportState.sType =
          VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      viewportState.viewportCount = 1;
      viewportState.pViewports = &viewport;
      viewportState.scissorCount = 1;
      viewportState.pScissors = &scissor;

      VkPipelineRasterizationStateCreateInfo rasterizer{};
      rasterizer.sType =
          VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      rasterizer.depthClampEnable = VK_FALSE;
      rasterizer.rasterizerDiscardEnable = VK_FALSE;
      rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
      rasterizer.lineWidth = 1.0f;
      rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
      rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
      rasterizer.depthBiasEnable = VK_FALSE;

      VkPipelineMultisampleStateCreateInfo multisampling{};
      multisampling.sType =
          VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      multisampling.sampleShadingEnable = VK_FALSE;
      multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

      VkPipelineColorBlendAttachmentState colorBlendAttachment{};
      colorBlendAttachment.colorWriteMask =
          VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
      colorBlendAttachment.blendEnable = VK_FALSE;

      VkPipelineColorBlendStateCreateInfo colorBlending{};
      colorBlending.sType =
          VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      colorBlending.logicOpEnable = VK_FALSE;
      colorBlending.logicOp = VK_LOGIC_OP_COPY;
      colorBlending.attachmentCount = 1;
      colorBlending.pAttachments = &colorBlendAttachment;
      colorBlending.blendConstants[0] = 0.0f;
      colorBlending.blendConstants[1] = 0.0f;
      colorBlending.blendConstants[2] = 0.0f;
      colorBlending.blendConstants[3] = 0.0f;

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = 0;
      pipelineLayoutInfo.pushConstantRangeCount = 0;

      std::string nmsg = "failed to create pipeline layout!";
      CHECK_VK(vkCreatePipelineLayout(myg.ldevice, &pipelineLayoutInfo, nullptr,
                                      &myg.pipeline_layout),
               nmsg, out.result_info);

      if (out.result_info != SUCCESS_OP) {
        out.next_node = 0;
        return out;
      }

      VkGraphicsPipelineCreateInfo pipelineInfo{};
      pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineInfo.stageCount = 2;
      pipelineInfo.pStages = shaderStages;
      pipelineInfo.pVertexInputState = &vertexInputInfo;
      pipelineInfo.pInputAssemblyState = &inputAssembly;
      pipelineInfo.pViewportState = &viewportState;
      pipelineInfo.pRasterizationState = &rasterizer;
      pipelineInfo.pMultisampleState = &multisampling;
      pipelineInfo.pColorBlendState = &colorBlending;
      pipelineInfo.layout = myg.pipeline_layout;
      pipelineInfo.renderPass = myg.render_pass;
      pipelineInfo.subpass = 0;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
      nmsg = "failed to create graphics pipeline!";
      CHECK_VK(vkCreateGraphicsPipelines(myg.ldevice, VK_NULL_HANDLE, 1,
                                         &pipelineInfo, nullptr,
                                         &myg.graphics_pipeline),
               nmsg, out.result_info);

      if (out.result_info.status != SUCCESS_OP) {
        out.next_node = 0;
        return out;
      }

      vkDestroyShaderModule(myg.ldevice, fragShaderModule, nullptr);
      vkDestroyShaderModule(myg.ldevice, vertShaderModule, nullptr);
      return out;
    };
    auto vr = mkAddNode<vk_triapp, 10, false, 11>(graph, f);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 10 with end node 11";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** create framebuffers node:
    depends on:
    - swap chain image views
    - swap chain extent
    - swap chain render pass
   */
  vk_node<vk_triapp> createFramebuffersNode;
  {
    createFramebuffersNode.is_singular = true;
    createFramebuffersNode.is_called = false;
    createFramebuffersNode.compute = [](vk_triapp &myg) {
      //
      myg.swapchain_framebuffers.resize(myg.views.size());

      for (size_t i = 0; i < myg.views.size(); i++) {
        VkImageView attachments[] = {myg.views[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = myg.render_pass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = myg.sextent.width;
        framebufferInfo.height = myg.sextent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(myg.ldevice, &framebufferInfo, nullptr,
                                &myg.swapchain_framebuffers[i]) != VK_SUCCESS) {
          std::cerr << "failed to create framebuffer!" << std::endl;
        }
      }
    };
    createFramebuffersNode.node_id = node_counter;
    node_counter++;
  }

  /** create command pool node:
    depends on:
    - logical device
    - surface
    - physical device
   */
  vk_node<vk_triapp> createCommandPoolNode;
  {
    createCommandPoolNode.is_singular = true;
    createCommandPoolNode.is_called = false;
    createCommandPoolNode.compute = [](vk_triapp &myg) {
      //
      QueueFamilyIndices queueFamilyIndices =
          findQueueFamilies(myg.pdevice, myg.surface);

      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

      if (vkCreateCommandPool(myg.ldevice, &poolInfo, nullptr, &myg.pool) !=
          VK_SUCCESS) {
        std::cerr << "failed to create command pool!" << std::endl;
      }
    };
    createCommandPoolNode.node_id = node_counter;
    node_counter++;
  }

  /** create command buffers node */
  vk_node<vk_triapp> createCommandAllocBuffersNode;
  {
    createCommandAllocBuffersNode.is_singular = true;
    createCommandAllocBuffersNode.is_called = false;
    createCommandAllocBuffersNode.compute = [](vk_triapp &myg) {
      //
      myg.cbuffers.resize(myg.swapchain_framebuffers.size());

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = myg.pool;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = (uint32_t)myg.cbuffers.size();
      for (unsigned int i = 0; i < myg.cbuffers.size(); i++) {
        myg.available_cbuffers[i] = true;
      }

      if (vkAllocateCommandBuffers(myg.ldevice, &allocInfo,
                                   myg.cbuffers.data()) != VK_SUCCESS) {
        std::cerr << "failed to allocate command buffers!" << std::endl;
        return;
      }
    };
    createCommandAllocBuffersNode.node_id = node_counter;
    node_counter++;
  }
  /** create command buffer node:
    depends on:
    - command allocation buffers node
    - swap chain framebuffers
    - render pass
    - graphics pipeline
    - swapchain extent
   */
  // a merging buffer which does not do anything
  vk_node<vk_triapp> cmdBufferMergeNode;
  {
    createCommandAllocBuffersNode.is_singular = true;
    createCommandAllocBuffersNode.is_called = false;
    createCommandAllocBuffersNode.compute = [](vk_triapp &myg) {};
    createCommandAllocBuffersNode.node_id = node_counter;
    node_counter++;
  }
  vk_node<vk_triapp> createCommandBufferNode;
  {
    createCommandBufferNode.is_singular = false;
    createCommandBufferNode.is_called = false;

    createCommandBufferNode.compute = [](vk_triapp &myg) {
      for (size_t i = 0; i < myg.cbuffers.size(); i++) {
        if (!myg.cbuffers[i]) {
          continue;
        }
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        // start recording commands
        if (vkBeginCommandBuffer(myg.cbuffers[i], &beginInfo) != VK_SUCCESS) {
          std::cerr << "failed to begin recording command buffer!"
                    << " buffer no " << i << std::endl;
          return;
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = myg.render_pass;
        renderPassInfo.framebuffer = myg.swapchain_framebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = myg.sextent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(myg.cbuffers[i], &renderPassInfo,
                             VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(myg.cbuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                          myg.graphics_pipeline);

        vkCmdDraw(myg.cbuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(myg.cbuffers[i]);

        if (vkEndCommandBuffer(myg.cbuffers[i]) != VK_SUCCESS) {
          std::cerr << "failed to record command buffer!"
                    << " buffer no " << i << std::endl;
        }
        // end recording commands
        // command buffer i is now in executable state
        myg.available_cbuffers[i] = false;
      }
    };
    createCommandBufferNode.node_id = node_counter;
    node_counter++;
  }

  vk_node<vk_triapp> createSyncObjectsNode;
  {
    createSyncObjectsNode.is_singular = false;
    createSyncObjectsNode.is_called = false;

    createSyncObjectsNode.compute = [](vk_triapp &myg) {
      //
      myg.image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
      myg.render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
      myg.current_fences.resize(MAX_FRAMES_IN_FLIGHT);
      myg.images_in_flight.resize(myg.simages.size(), VK_NULL_HANDLE);

      VkSemaphoreCreateInfo semaphoreInfo{};
      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      VkFenceCreateInfo fenceInfo{};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(myg.ldevice, &semaphoreInfo, nullptr,
                              &myg.image_available_semaphores[i]) !=
                VK_SUCCESS ||
            vkCreateSemaphore(myg.ldevice, &semaphoreInfo, nullptr,
                              &myg.render_finished_semaphores[i]) !=
                VK_SUCCESS ||
            vkCreateFence(myg.ldevice, &fenceInfo, nullptr,
                          &myg.current_fences[i]) != VK_SUCCESS) {
          std::cerr << "failed to create synchronization objects for a frame!"
                    << std::endl;
          return;
        }
      }
    };
    createSyncObjectsNode.node_id = node_counter;
    node_counter++;
  }

  vk_node<vk_triapp> renderLoopNode;
  {
    renderLoopNode.is_singular = false;
    renderLoopNode.is_called = false;

    renderLoopNode.compute = [](vk_triapp &myg) {
      //
      while (!glfwWindowShouldClose(myg.window)) {
        glfwPollEvents();
        drawFrame(myg);
      }

      vkDeviceWaitIdle(myg.ldevice);
    };
    renderLoopNode.node_id = node_counter;
    node_counter++;
  }

  // run first edge

  std::cout << "everything runs" << std::endl;

  // start declaring nodes
  return 0;
}
