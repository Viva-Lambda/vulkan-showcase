#pragma once

// graph like architecture
#include <external.hpp>
#include <initvk/vkapp.hpp>
#include <initvk/vkinstance.hpp>
#include <vertex.hpp>
#include <vkdebug/debug.hpp>
#include <vkdevice/physical.hpp>
#include <vkgraph/vknode.hpp>
#include <vkimageview/imageview.hpp>
#include <vkqueuefamily/index.hpp>
#include <vkqueuefamily/queue.hpp>
#include <vkswapchain/support.hpp>
#include <vkswapchain/swapchain.hpp>

namespace vtuto {

struct vk_triapp {

  /** Attributes unrelated to vulkan
    @{
   */
  std::string win_title = "Vulkan Window Graph";
  uint32_t win_width = 800;
  uint32_t win_height = 600;

  GLFWwindow *window; // window for visualizing object.

  /** @} */

  /** instance of the vulkan application */
  VkInstance instance;

  /** debug callback function handler */
  VkDebugUtilsMessengerEXT debugMessenger;

  /** physical device pointer and related objects
   * @{
   */

  /** physical device handler */
  VkPhysicalDevice pdevice = VK_NULL_HANDLE;
  /** physical device windows surface */
  VkSurfaceKHR surface;
  /** @} */

  /** logical device pointer and related objects
    @{
   */

  /** logical device handler */
  VkDevice ldevice;

  /** queues */
  /** graphics queue */
  queue_map queues = {std::make_pair(VK_QUEUE_GRAPHICS_BIT, VkQueue{})};

  /** window surface queue */
  VkQueue present_queue;
  /** @} */

  /** swap chain for handling frame rate
    It contains several handlers all dealing with
    frames
    @{
   */
  /** swapchain for handling frame rate*/
  VkSwapchainKHR chain;

  /** images in swap chain */
  std::vector<VkImage> simages;

  /** swapchain image format*/
  VkFormat simage_format;

  /** swapchain extent*/
  VkExtent2D sextent;

  /** swapchain image view */

  std::vector<VkImageView> views;

  /** @} */

  /** swap chain frame buffers
    @{
   */
  std::vector<VkFramebuffer> swapchain_framebuffers;

  /** @} */

  /** render pass @{ */

  VkRenderPass render_pass;

  /** @} */

  /** descriptor set layout @{ */
  VkDescriptorSetLayout descriptor_set_layout;

  /** @} */

  /** descriptor pool @{*/
  VkDescriptorPool descriptor_pool;

  /** @} */

  /** descriptor sets @{*/

  std::vector<VkDescriptorSet> descriptor_sets;

  /** @} */

  /** graphics pipeline layout @{*/
  VkPipelineLayout pipeline_layout;

  /** @} */

  /** graphics pipeline object @{*/

  VkPipeline graphics_pipeline;

  /** @} */

  /** command pool and command buffer handlers

    From spec: Command pools are externally synchronized,
    meaning that a
    command pool must not be used concurrently in multiple
    threads. That
    includes use via recording commands on any command
    buffers allocated from
    the pool, as well as operations that allocate, free, and
    reset command
    buffers or the pool itself.

    @{*/

  VkCommandPool pool;
  std::vector<VkCommandBuffer> cbuffers;
  std::vector<bool> available_cbuffers;

  /** @} */

  /** Texture related handlers @{*/

  /** staging buffer */

  VkBuffer staging_buffer;
  VkDeviceMemory stage_buffer_memory;
  VkImage texture_image;
  VkDeviceMemory texture_image_memory;

  /** texture image view */
  VkImageView texture_image_view;

  /** texture sampler */
  VkSampler texture_sampler;

  /** @} */

  /** depth image related @{*/
  VkImage depth_image;
  VkImageView depth_image_view;
  VkDeviceMemory depth_image_memory;

  /** @} */

  /** Scene dependent attributes
    @{
   */

  /** vertices per scene and indices per scene */

  std::vector<Vertex> vertices;
  std::vector<std::uint32_t> indices;

  /** vertex buffer */
  VkBuffer vertex_buffer;
  VkDeviceMemory vertex_buffer_memory;

  /** index buffer */
  VkBuffer index_buffer;
  VkDeviceMemory index_buffer_memory;

  /** uniform buffer */
  std::vector<VkBuffer> uniform_buffers;
  std::vector<VkDeviceMemory> uniform_buffer_memories;

  /** @} */

  /** Thread related objects and handlers
    @{
   */

  /** vk semaphore to hold available and rendered images */
  std::vector<VkSemaphore> image_available_semaphores;
  std::vector<VkSemaphore> render_finished_semaphores;

  /** fence image for drawing */
  std::vector<VkFence> current_fences;
  std::vector<VkFence> images_in_flight;
  std::size_t current_frame = 0;

  /** maximum frames in flight*/
  const int MAX_FRAMES_IN_FLIGHT = 2;

  /** @} */

  /** check framebuffer state*/
  bool framebuffer_resized = false;
};

static void framebufferResizeCallback(GLFWwindow *window, int width,
                                      int height) {
  auto app = reinterpret_cast<vk_triapp *>(glfwGetWindowUserPointer(window));
  app->framebuffer_resized = true;
}
const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

VkResult CreateDebugUtilsMessengerEXT(
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

void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  auto severities = concat_bits<VkDebugUtilsMessageSeverityFlagBitsEXT>(
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

  auto mtypes = concat_bits<VkDebugUtilsMessageTypeFlagBitsEXT>(
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);

  createInfo = mkDebugMessengerCreateInfo(severities, mtypes, debugCallback);
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

std::unordered_map<std::string, std::function<vk_output(vk_triapp &)>>
vk_triAppFns() {
  //
  std::unordered_map<std::string, std::function<vk_output(vk_triapp &)>> fm;
  /** init window node:
      - node id 1
      - target nodes: {2: createInstance}
   */
  fm["initWindow"] = [](vk_triapp &myg) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    myg.window = glfwCreateWindow(myg.win_width, myg.win_height, "Vulkan",
                                  nullptr, nullptr);
    glfwSetWindowUserPointer(myg.window, &myg);
    glfwSetFramebufferSizeCallback(myg.window, framebufferResizeCallback);
    vk_output out;
    out.signal = 1;
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    out.result_info = vr;
    return out;
  };
  //
  fm["createInstance"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    //
    if (enableValidationLayers && !checkValidationLayerSupport()) {
      vr.context = "validation layers requested, but not available!";
      out.signal = 0;
      vr.status = FAIL_OP;
      out.result_info = vr;
      return out;
    }
    EngineVersion ev(1, 0, 0);
    AppVersion av(1, 0, 0);
    VkApplicationInfo appInfo = mkAppInfo<VK_API_VERSION_1_0>(
        ev, av, "Hello Triangle", "No Engine", NULL);

    auto extensions = getRequiredExtensions();
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    populateDebugMessengerCreateInfo(debugCreateInfo);
    auto dcinfo =
        static_cast<VkDebugUtilsMessengerCreateInfoEXT>(debugCreateInfo);
    VkInstanceCreateInfo createInfo =
        mkInstanceCreateInfo(appInfo, extensions, validationLayers, dcinfo);

    std::string nmsg = "instance creation failed";
    CHECK_VK(vkCreateInstance(&createInfo, nullptr, &myg.instance), nmsg, vr);
    out.result_info = vr;
    out.signal = 1;
    return out;
  };
  //
  fm["setupDebugMessenger"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    if (!enableValidationLayers) {
      return out;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    std::string nmsg = "failed to set up debug messenger!";
    CHECK_VK(CreateDebugUtilsMessengerEXT(myg.instance, &createInfo, nullptr,
                                          &myg.debugMessenger),
             nmsg, out.result_info);
    if (out.result_info.status != SUCCESS_OP) {
      out.signal = 0;
    }
    return out;
  };
  //
  fm["createSurface"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;
    std::string nmsg = "failed to create window surface!";

    CHECK_VK(glfwCreateWindowSurface(myg.instance, myg.window, nullptr,
                                     &myg.surface),
             nmsg, out.result_info);
    if (out.result_info.status != SUCCESS_OP) {
      out.signal = 0;
    }
    return out;
  };
  //
  fm["pickPhysicalDevice"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(myg.instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
      out.result_info.status = FAIL_OP;
      out.result_info.context = "failed to find GPUs with Vulkan support!";
      out.signal = 0;
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
      out.signal = 0;
      return out;
    }
    return out;
  };
  //
  fm["createLogicalDevice"] = [](vk_triapp &myg) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    QueueFamilyIndices indices =
        find_queue_families<VK_QUEUE_GRAPHICS_BIT>(myg.pdevice, myg.surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    auto indice_values = indices.values();
    std::set<uint32_t> uniqueQueueFamilies(indice_values.begin(),
                                           indice_values.end());

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

    std::array<VkExtensionProperties, 1> arr = {
        {{VK_KHR_SWAPCHAIN_EXTENSION_NAME, 1}}};
    PhysicalDeviceExtensionQuery pext(arr);

    createInfo.enabledExtensionCount = pext.size();
    auto char_arr = pext.cdata();
    auto vptr = char_arr.data();
    createInfo.ppEnabledExtensionNames = vptr->data();

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
      out.signal = 0;
      return out;
    }

    uint32_t graphics_family = 0;
    indices.index<VK_QUEUE_GRAPHICS_BIT>(graphics_family);

    vkGetDeviceQueue(myg.ldevice, graphics_family, 0,
                     &myg.queues[VK_QUEUE_GRAPHICS_BIT]);
    vkGetDeviceQueue(myg.ldevice, indices.presentFamily.value(), 0,
                     &myg.present_queue);
    return out;
  };
  //
  fm["createSwapChain"] = [](vk_triapp &myg) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    VkSurfaceFormatKHR surfaceFormat;
    VkExtent2D extent;
    std::uint32_t imageCount = 0;

    constexpr VkFormat SCFormat = VK_FORMAT_B8G8R8A8_SRGB;
    constexpr VkColorSpaceKHR SCColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    constexpr VkImageUsageFlags SCImgUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    constexpr VkCompositeAlphaFlagBitsKHR SCAlphaBits =
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    constexpr VkBool32 IsClipped = VK_TRUE;
    constexpr unsigned int SCImageArrayLayers = 1;

    SwapchainCreateInfoKHR_VK scinfo;
    scinfo.set<SCFormat, SCColorSpace, SCImgUsage, SCAlphaBits, IsClipped,
               SCImageArrayLayers, VK_QUEUE_GRAPHICS_BIT>(
        myg.pdevice, myg.surface, myg.window, surfaceFormat, extent,
        imageCount);

    std::string nmsg = "failed to create swap chain!";
    CHECK_VK(
        vkCreateSwapchainKHR(myg.ldevice, &scinfo.createInfo, nullptr, &myg.chain),
        nmsg, out.result_info);

    if (out.result_info.status != SUCCESS_OP) {
      out.signal = 0;
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
  //
  fm["createImageViews"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    myg.views.resize(myg.simages.size());

    for (size_t i = 0; i < myg.simages.size(); i++) {
      VkImageViewCreateInfo createInfo{};
      //
      ImageViewCreateInfo_VK imview_create;
      imview_create.set<VK_IMAGE_VIEW_TYPE_2D,         // ImageViewType
                        VK_FORMAT_B8G8R8A8_SRGB,       // ImageFormat
                        VK_COMPONENT_SWIZZLE_IDENTITY, // R
                        VK_COMPONENT_SWIZZLE_IDENTITY, // G
                        VK_COMPONENT_SWIZZLE_IDENTITY, // B
                        VK_COMPONENT_SWIZZLE_IDENTITY, // A
                        1,                             // LayerCount
                        0,                             // BaseArrayLayer
                        0,                             // BaseMipLevel
                        1,                             // LevelCount
                        VK_IMAGE_ASPECT_COLOR_BIT      // aspect flag
                        >(myg.simages[i]);
      createInfo = imview_create.createInfo;
      std::string nmsg = "failed to create image views!";
      CHECK_VK(
          vkCreateImageView(myg.ldevice, &createInfo, nullptr, &myg.views[i]),
          nmsg, out.result_info);

      if (out.result_info.status != SUCCESS_OP) {
        out.signal = 0;
        return out;
      }
    }
    return out;
  };
  //
  fm["createRenderPass"] = [](vk_triapp &myg) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

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
      out.signal = 0;
    }
    return out;
  };
  //
  fm["createGraphicsPipeline"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

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
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
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

    if (out.result_info.status != SUCCESS_OP) {
      out.signal = 0;
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
      out.signal = 0;
      return out;
    }

    vkDestroyShaderModule(myg.ldevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(myg.ldevice, vertShaderModule, nullptr);
    return out;
  };
  //
  fm["createFramebuffers"] = [](vk_triapp &myg) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

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

      std::string nmsg = "failed to create framebuffer!";

      CHECK_VK(vkCreateFramebuffer(myg.ldevice, &framebufferInfo, nullptr,
                                   &myg.swapchain_framebuffers[i]),
               nmsg, out.result_info);

      if (out.result_info.status != SUCCESS_OP) {
        out.signal = 0;
        return out;
      }
    }
    return out;
  };
  //
  fm["createCommandPool"] = [](vk_triapp &myg) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    QueueFamilyIndices indices =
        find_queue_families<VK_QUEUE_GRAPHICS_BIT>(myg.pdevice, myg.surface);

    uint32_t graphicsFamily = 0;
    indices.index<VK_QUEUE_GRAPHICS_BIT>(graphicsFamily);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsFamily;

    std::string nmsg = "failed to create command pool!";

    CHECK_VK(vkCreateCommandPool(myg.ldevice, &poolInfo, nullptr, &myg.pool),
             nmsg, out.result_info);

    if (out.result_info.status != SUCCESS_OP) {
      out.signal = 0;
    }
    return out;
  };
  //
  fm["createCommandBufferAlloc"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    //
    myg.cbuffers.resize(myg.swapchain_framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = myg.pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)myg.cbuffers.size();
    myg.available_cbuffers.resize(myg.cbuffers.size());
    for (unsigned int i = 0; i < myg.cbuffers.size(); i++) {
      myg.available_cbuffers[i] = true;
    }

    std::string nmsg = "failed to allocate command buffers!";
    CHECK_VK(
        vkAllocateCommandBuffers(myg.ldevice, &allocInfo, myg.cbuffers.data()),
        nmsg, out.result_info);

    if (out.result_info.status != SUCCESS_OP) {
      out.signal = 0;
    }
    return out;
  };
  //
  fm["createCommandBuffers"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    for (size_t i = 0; i < myg.cbuffers.size(); i++) {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      std::string nmsg = "failed to begin recording command buffer!";
      nmsg += " buffer no ";
      nmsg += std::to_string(i);

      CHECK_VK(vkBeginCommandBuffer(myg.cbuffers[i], &beginInfo), nmsg,
               out.result_info);

      // start recording commands
      if (out.result_info.status != SUCCESS_OP) {
        out.signal = 0;
        return out;
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

      nmsg = "failed to record command buffer!";
      nmsg += " buffer no ";
      nmsg += std::to_string(i);
      CHECK_VK(vkEndCommandBuffer(myg.cbuffers[i]), nmsg, out.result_info);

      if (out.result_info.status != SUCCESS_OP) {
        out.signal = 0;
        return out;
      }
    }
    return out;
  };
  //
  fm["createSyncObjects"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;

    //
    myg.image_available_semaphores.resize(myg.MAX_FRAMES_IN_FLIGHT);
    myg.render_finished_semaphores.resize(myg.MAX_FRAMES_IN_FLIGHT);
    myg.current_fences.resize(myg.MAX_FRAMES_IN_FLIGHT);
    myg.images_in_flight.resize(myg.simages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    auto max_frames = static_cast<std::size_t>(myg.MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < max_frames; i++) {
      std::string nmsg =
          "failed to create synchronization objects for a frame!";
      nmsg += " frame ";
      nmsg += std::to_string(i);

      CHECK_VK(vkCreateSemaphore(myg.ldevice, &semaphoreInfo, nullptr,
                                 &myg.image_available_semaphores[i]),
               nmsg, out.result_info);
      if (out.result_info.status != SUCCESS_OP) {
        out.signal = 0;
        return out;
      }

      CHECK_VK(vkCreateSemaphore(myg.ldevice, &semaphoreInfo, nullptr,
                                 &myg.render_finished_semaphores[i]),
               nmsg, out.result_info);
      if (out.result_info.status != SUCCESS_OP) {
        out.signal = 0;
        return out;
      }

      CHECK_VK(vkCreateFence(myg.ldevice, &fenceInfo, nullptr,
                             &myg.current_fences[i]),
               nmsg, out.result_info);
      if (out.result_info.status != SUCCESS_OP) {
        out.signal = 0;
        return out;
      }
    }
    return out;
  };
  //
  fm["windowShouldClose"] = [](vk_triapp &myg) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;
    //
    if (!glfwWindowShouldClose(myg.window)) {
      glfwPollEvents();
      return out;
    }
    vkDeviceWaitIdle(myg.ldevice);
    out.signal = 2;
    return out;
  };
  fm["drawFrame"] = [](vk_triapp &g) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;
    //
    vkWaitForFences(g.ldevice, 1, &g.current_fences[g.current_frame], VK_TRUE,
                    UINT64_MAX);

    uint32_t imageIndex;
    std::string nmsg = "failed to acquire swap chain image!";
    CHECK_VK(
        vkAcquireNextImageKHR(g.ldevice, g.chain, UINT64_MAX,
                              g.image_available_semaphores[g.current_frame],
                              VK_NULL_HANDLE, &imageIndex),
        nmsg, out.result_info);

    if (out.result_info.result == VK_ERROR_OUT_OF_DATE_KHR) {
      // recreateSwapChain
      out.signal = 2;
      return out;
    } else if (out.result_info.result != VK_SUCCESS &&
               out.result_info.result != VK_SUBOPTIMAL_KHR) {
      out.signal = 0;
      return out;
    }

    if (g.images_in_flight[imageIndex] != VK_NULL_HANDLE) {
      vkWaitForFences(g.ldevice, 1, &g.images_in_flight[imageIndex], VK_TRUE,
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

    nmsg = "failed to submit draw command buffer!";
    CHECK_VK(vkQueueSubmit(g.queues[VK_QUEUE_GRAPHICS_BIT], 1, &submitInfo,
                           g.current_fences[g.current_frame]),
             nmsg, out.result_info);

    if (out.result_info.status != SUCCESS_OP) {
      out.signal = 0;
      return out;
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {g.chain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    nmsg = "failed to present swap chain image!";
    CHECK_VK(vkQueuePresentKHR(g.present_queue, &presentInfo), nmsg,
             out.result_info);

    bool c1 = out.result_info.result == VK_ERROR_OUT_OF_DATE_KHR;
    bool c2 = out.result_info.result == VK_SUBOPTIMAL_KHR;

    if (c1 || c2 || g.framebuffer_resized) {
      g.framebuffer_resized = false;
      // recreateSwapChain
      out.signal = 2;
    } else if (out.result_info.status != SUCCESS_OP) {
      out.signal = 0;
      return out;
    }

    g.current_frame = (g.current_frame + 1) % g.MAX_FRAMES_IN_FLIGHT;

    return out;
  };
  //
  fm["recreateSwapChain"] = [](vk_triapp &myg) {
    //
    int width = 0, height = 0;
    glfwGetFramebufferSize(myg.window, &width, &height);
    while (width == 0 || height == 0) {
      glfwGetFramebufferSize(myg.window, &width, &height);
      glfwWaitEvents();
    }
    vkDeviceWaitIdle(myg.ldevice);
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;
    return out;
  };
  // cleanup swap chain
  fm["cleanupSwapChain"] = [](vk_triapp &myg) {
    for (auto framebuffer : myg.swapchain_framebuffers) {
      vkDestroyFramebuffer(myg.ldevice, framebuffer, nullptr);
    }

    vkFreeCommandBuffers(myg.ldevice, myg.pool,
                         static_cast<uint32_t>(myg.cbuffers.size()),
                         myg.cbuffers.data());

    vkDestroyPipeline(myg.ldevice, myg.graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(myg.ldevice, myg.pipeline_layout, nullptr);
    vkDestroyRenderPass(myg.ldevice, myg.render_pass, nullptr);

    for (auto imageView : myg.views) {
      vkDestroyImageView(myg.ldevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(myg.ldevice, myg.chain, nullptr);

    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;
    return out;
  };
  //
  fm["imagesInFlightResize"] = [](vk_triapp &myg) {
    myg.images_in_flight.resize(myg.simages.size(), VK_NULL_HANDLE);
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;
    return out;
  };
  //
  fm["destroyAll"] = [](vk_triapp &myg) {
    for (size_t i = 0; i < myg.MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(myg.ldevice, myg.render_finished_semaphores[i],
                         nullptr);
      vkDestroySemaphore(myg.ldevice, myg.image_available_semaphores[i],
                         nullptr);
      vkDestroyFence(myg.ldevice, myg.current_fences[i], nullptr);
    }

    vkDestroyCommandPool(myg.ldevice, myg.pool, nullptr);

    vkDestroyDevice(myg.ldevice, nullptr);

    if (enableValidationLayers) {
      DestroyDebugUtilsMessengerEXT(myg.instance, myg.debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(myg.instance, myg.surface, nullptr);
    vkDestroyInstance(myg.instance, nullptr);

    glfwDestroyWindow(myg.window);

    glfwTerminate();
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    vk_output out;
    out.result_info = vr;
    out.signal = 1;
    return out;
  };

  return fm;
}
} // namespace vtuto
