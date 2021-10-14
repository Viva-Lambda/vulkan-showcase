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

  /** vulkan create logical device node*/
  vk_node createLogicalDeviceNode;
  {
    createLogicalDeviceNode.is_singular = true;
    createLogicalDeviceNode.is_called = false;
    createLogicalDeviceNode.compute = [](vk_graph &myg) {
      //
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

      if (vkCreateDevice(myg.pdevice, &createInfo, nullptr, &myg.ldevice) !=
          VK_SUCCESS) {
        std::cerr << "failed to create logical device!" << std::endl;
        return;
      }

      vkGetDeviceQueue(myg.ldevice, indices.graphicsFamily.value(), 0,
                       &myg.graphics_queue);
      vkGetDeviceQueue(myg.ldevice, indices.presentFamily.value(), 0,
                       &myg.present_queue);
    };
    createLogicalDeviceNode.node_id = node_counter;
    node_counter++;
  }

  /** vulkan create swap chain node*/
  vk_node createSwapChainNode;
  {
    createSwapChainNode.is_singular = true;
    createSwapChainNode.is_called = false;
    createSwapChainNode.compute = [](vk_graph &myg) {
      //
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

      if (vkCreateSwapchainKHR(myg.ldevice, &createInfo, nullptr, &myg.chain) !=
          VK_SUCCESS) {
        std::cerr << "failed to create swap chain!" << std::endl;
      }

      vkGetSwapchainImagesKHR(myg.ldevice, myg.chain, &imageCount, nullptr);
      myg.simages.resize(imageCount);
      vkGetSwapchainImagesKHR(myg.ldevice, myg.chain, &imageCount,
                              myg.simages.data());

      myg.simage_format = surfaceFormat.format;
      myg.sextent = extent;
    };
    createSwapChainNode.node_id = node_counter;
    node_counter++;
  }
  /** create swap chain image views*/
  vk_node createImageViewsNode;
  {
    createImageViewsNode.is_singular = true;
    createImageViewsNode.is_called = false;
    createImageViewsNode.compute = [](vk_graph &myg) {
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

        if (vkCreateImageView(myg.ldevice, &createInfo, nullptr,
                              &myg.views[i]) != VK_SUCCESS) {
          std::cerr << "failed to create image views!" << std::endl;
        }
      }
    };
    createImageViewsNode.node_id = node_counter;
    node_counter++;
  }
  /** create render pass for all the images*/
  vk_node createRenderPassNode;
  {
    createRenderPassNode.is_singular = true;
    createRenderPassNode.is_called = false;
    createRenderPassNode.compute = [](vk_graph &myg) {
      //
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

      if (vkCreateRenderPass(myg.ldevice, &renderPassInfo, nullptr,
                             &myg.render_pass) != VK_SUCCESS) {
        std::cerr << "failed to create render pass!" << std::endl;
      }
    };
    createRenderPassNode.node_id = node_counter;
    node_counter++;
  }
  /** create graphics pipeline */
  vk_node createGraphicsPipelineNode;
  {
    createGraphicsPipelineNode.is_singular = true;
    createGraphicsPipelineNode.is_called = false;
    createGraphicsPipelineNode.compute = [](vk_graph &myg) {
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

      if (vkCreatePipelineLayout(myg.ldevice, &pipelineLayoutInfo, nullptr,
                                 &myg.pipeline_layout) != VK_SUCCESS) {
        std::cerr << "failed to create pipeline layout!" << std::endl;
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

      if (vkCreateGraphicsPipelines(myg.ldevice, VK_NULL_HANDLE, 1,
                                    &pipelineInfo, nullptr,
                                    &myg.graphics_pipeline) != VK_SUCCESS) {
        std::cerr << "failed to create graphics pipeline!" << std::endl;
      }

      vkDestroyShaderModule(myg.ldevice, fragShaderModule, nullptr);
      vkDestroyShaderModule(myg.ldevice, vertShaderModule, nullptr);
    };
    createGraphicsPipelineNode.node_id = node_counter;
    node_counter++;
  }

  /** create framebuffers node:
    depends on:
    - swap chain image views
    - swap chain extent
    - swap chain render pass
   */
  vk_node createFramebuffersNode;
  {
    createFramebuffersNode.is_singular = true;
    createFramebuffersNode.is_called = false;
    createFramebuffersNode.compute = [](vk_graph &myg) {
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
  vk_node createCommandPoolNode;
  {
    createCommandPoolNode.is_singular = true;
    createCommandPoolNode.is_called = false;
    createCommandPoolNode.compute = [](vk_graph &myg) {
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
  vk_node createCommandAllocBuffersNode;
  {
    createCommandAllocBuffersNode.is_singular = true;
    createCommandAllocBuffersNode.is_called = false;
    createCommandAllocBuffersNode.compute = [](vk_graph &myg) {
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
  vk_node cmdBufferMergeNode;
  {
    createCommandAllocBuffersNode.is_singular = true;
    createCommandAllocBuffersNode.is_called = false;
    createCommandAllocBuffersNode.compute = [](vk_graph &myg) {};
    createCommandAllocBuffersNode.node_id = node_counter;
    node_counter++;
  }
  vk_node createCommandBufferNode;
  {
    createCommandBufferNode.is_singular = false;
    createCommandBufferNode.is_called = false;

    createCommandBufferNode.compute = [](vk_graph &myg) {
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

  vk_node createSyncObjectsNode;
  {
    createSyncObjectsNode.is_singular = false;
    createSyncObjectsNode.is_called = false;

    createSyncObjectsNode.compute = [](vk_graph &myg) {
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

  // edges of the graph
  unsigned int edge_counter = 1;

  mkAddVkGEdge(graph, init_w_node, createInstanceNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createInstanceNode, setupDebugMessengerNode,
               edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createInstanceNode, createSurfaceNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createSurfaceNode, pickPhysicalDeviceNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, pickPhysicalDeviceNode, createLogicalDeviceNode,
               edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createLogicalDeviceNode, createSwapChainNode,
               edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createSwapChainNode, createImageViewsNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createSwapChainNode, createRenderPassNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createRenderPassNode, createGraphicsPipelineNode,
               edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createRenderPassNode, createFramebuffersNode,
               edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createLogicalDeviceNode, createCommandPoolNode,
               edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createCommandPoolNode, createCommandAllocBuffersNode,
               edge_counter);
  edge_counter++;

  // merge node for specifying dependencies of a command buffer
  mkAddVkGEdge(graph, createCommandAllocBuffersNode, cmdBufferMergeNode,
               edge_counter);
  edge_counter++;
  //

  mkAddVkGEdge(graph, createSwapChainNode, cmdBufferMergeNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createFramebuffersNode, cmdBufferMergeNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createRenderPassNode, cmdBufferMergeNode, edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createGraphicsPipelineNode, cmdBufferMergeNode,
               edge_counter);
  edge_counter++;

  // now add the create command buffer node
  mkAddVkGEdge(graph, cmdBufferMergeNode, createCommandBufferNode,
               edge_counter);
  edge_counter++;

  mkAddVkGEdge(graph, createCommandBufferNode, createSyncObjectsNode,
               edge_counter);
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
