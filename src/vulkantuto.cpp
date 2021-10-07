// main file
#include <debug.hpp>
#include <external.hpp>
#include <hellotriangle.hpp>
#include <ldevice.hpp>
#include <pdevice.hpp>
#include <support.hpp>
#include <triangle.hpp>
#include <ubo.hpp>
#include <utils.hpp>
// #include <vkdefault/instancedefaults.hpp>
//
using namespace vtuto;

namespace vtuto {

/**
  @brief Hello Triangle application object.

  The application contains most of the steps for dealing
  with
  vulkan applications.
 */

/**
Run application.

Steps to run the application
*/
void HelloTriangle::run() {
  // 1. launch window
  initWindow();

  // 2. launch vulkan
  initVulkan();

  // 3. main loop
  renderLoop();

  // 4. clean up ressources
  cleanUp();
}

/**
  Initialize window.

  Gives window hints. Sets its size, its title, etc.
 */
void HelloTriangle::initWindow() {
  if (glfwInit() == 0) {
    throw std::runtime_error("Unable to launch glfw window");
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  //
  window = glfwCreateWindow(win_width, win_height, win_title.c_str(), nullptr,
                            nullptr);
  glfwSetWindowUserPointer(window, nullptr);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
}
/**
Initialize vulkan.

Steps to initialize a vulkan api
1. Create a vulkan instance
*/
void HelloTriangle::initVulkan() {
  //
  /**
    1. Create a vulkan instance
    Define api version, required extensions by the application,
    enable validation layers
   */
  createInstance();

  // 2. Setup debug messenger
  /**
    Specify the severity of validation layers:
    - which info should be shown
    - what should be warning level
    - etc.
   */
  setupDebugMessenger();

  /**
    3. Create physical device
    Create physical device by creating the surface from glfw window.
    Then we enumerate available physical devices in the host machine. This
    does not necessarily mean having multiple gpu's, it is possible that the
    cpu also understands vulkan (intel?), however the major use case for this
    is having multiple gpus.
    Each devices is tested against a set of conditions. These conditions are
    determined by the nature of our application. The criteria is specified in
    the body of is_device_suitable() function.
   */
  physical_dev = vulkan_device<VkPhysicalDevice>(&instance, window);

  /** 4. Create logical device
   */
  logical_dev = vulkan_device<VkDevice>(enableValidationLayers, physical_dev);

  // 5. create swap chain
  swap_chain = swapchain(physical_dev, logical_dev, window);

  /**
    7. create render pass

    Specify attachments to be used in render pass.
    Essentially a color attachment and a depth attachment is created.
    Then these are passed to subpass description struct. This struct is then
    passed to a subpass dependency struct.
    The dependency struct takes flags about when to use the subpass, what is
    it can access to, etc.
    Then we attach attachments and the subpass struct to a renderpass info.
    The renderpass info then is used to create a render pass from logical
    device
   */
  createRenderPass();

  // 8. descriptor set layout
  createDescriptorSetLayout();

  // 8. create graphics pipeline
  createGraphicsPipeline();

  // 9. create graphics pipeline
  createDepthRessources();

  // 10. create framebuffers
  createFramebuffers();

  // 11. create command pool
  // createCommandPool();
  command_pool = vk_command_pool(physical_dev, logical_dev);

  // 12. create depth image
  // createDepthRessources();

  // 13. create texture images
  createTextureImage();

  // 14. create texture image viewer
  createTextureImageView();

  // 15. create texture sampler
  createTextureSampler();

  loadModel();

  // 16. create vertex buffer
  createVertexBuffer();

  // 17. create index buffer
  createIndexBuffer();

  // 18. create uniform buffers
  createUniformBuffer();

  // 19. create descriptor pool
  createDescriptorPool();

  // 20. create descriptor sets
  createDescriptorSets();

  // 21. create command buffer
  createCommandBuffers();

  // 22. create sync objects: semaphores, fences etc
  createSyncObjects();
}

// void HelloTriangle::createInstance() {
//
//    mkInstance(instance);
//}
/**
  Rendering loop.

  Render elements to window. Acquire user input
 */
void HelloTriangle::renderLoop() {
  //
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    draw();
  }
  vkDeviceWaitIdle(logical_dev.device());
}
/**
  Clean up ressources.

  Destroy window, and other ressources.
 */
void HelloTriangle::cleanUp() {
  //
  auto v = cmd_buffers.to_vec();
  swap_chain.destroy(logical_dev, command_pool.pool, v, swapchain_framebuffers,
                     render_pass, graphics_pipeline, pipeline_layout,
                     uniform_buffers, uniform_buffer_memories, descriptor_pool,
                     depth_image, depth_image_view, depth_image_memory);

  // destroy texture sampler
  vkDestroySampler(logical_dev.device(), texture_sampler, nullptr);
  // destroy image view
  vkDestroyImageView(logical_dev.device(), texture_image_view, nullptr);
  //
  vkDestroyImage(logical_dev.device(), texture_image, nullptr);
  vkFreeMemory(logical_dev.device(), texture_image_memory, nullptr);
  //
  vkDestroyDescriptorSetLayout(logical_dev.device(), descriptor_set_layout,
                               nullptr);

  vkDestroyBuffer(logical_dev.device(), index_buffer, nullptr);
  vkFreeMemory(logical_dev.device(), index_buffer_memory, nullptr);

  vkDestroyBuffer(logical_dev.device(), vertex_buffer, nullptr);
  vkFreeMemory(logical_dev.device(), vertex_buffer_memory, nullptr);
  auto mxflight = static_cast<unsigned int>(MAX_FRAMES_IN_FLIGHT);

  for (unsigned int i = 0; i < mxflight; i++) {
    vkDestroySemaphore(logical_dev.device(), render_finished_semaphores[i],
                       nullptr);
    vkDestroySemaphore(logical_dev.device(), image_available_semaphores[i],
                       nullptr);
    vkDestroyFence(logical_dev.device(), current_fences[i], nullptr);
  }
  command_pool.destroy(logical_dev);

  // 4. destroy logical device
  logical_dev.destroy();
  // 5. destroy debugging utils
  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  }
  // 6. destroy surface
  physical_dev.destroy();

  // 7. destroy instance always last in
  // a vulkan application.
  vkDestroyInstance(instance, nullptr);

  // 8. destroy window
  glfwDestroyWindow(window);

  // 9. glfw terminate
  glfwTerminate();
}
/**
  Check if requested layers are available

  Validation layers come with sdk they are not supported
  by Vulkan by
  default. We check if the requested layers are found in
  the system. In
  order to do that we first check the instance's layer
  properties with \c
  vkEnumerateInstanceLayerProperties() \c function.
  Requested layers are
  in requested_validation_layers vector. We compare the
  name of the
  available layers in the instance and members of
  requested_validation_layers.
 */
bool HelloTriangle::checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);

  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : requested_validation_layers) {
    //
    bool requestedLayerIsFound = false;
    for (const auto &layerProperties : availableLayers) {
      //
      std::string layer_name = layerName;
      std::string available_layer_name = layerProperties.layerName;
      if (layer_name == available_layer_name) {
        requestedLayerIsFound = true;
        break;
      }
    }
    if (!requestedLayerIsFound) {
      return false;
    }
  }
  //
  return true;
}
/**
  Specify properties of the debug messenger callback

  We add its type, requested message severities, message
  types, and we add which debug callback function is going
  to be used
 */
void HelloTriangle::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  //
  /**
    Determines message severity. If null, messages are
skipped

VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: specifies
the most verbose
output from validation layers

VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: specifies
messages that *may*
indicate an api bug.

VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: outputs error
message when the
api has violated a valid usage condition of the
specification.
   */
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  /**
VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: specifies if
some general event
has occured

VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: specifies if
some event has
occured during the validation stage against the vulkan
specification, meaning
that the application has either an undefined behaviour or an
error.

VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: specifies
if a potential
suboptimal use of Vulkan API is happening in the
application.
   */
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  //
  /**
pfnUserCallback: specified as a static function in this api.
typedef VkBool32 (VKAPI_PTR
*PFN_vkDebugUtilsMessangerCallbackEXT
)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*
pCallbackData,
    void* pUserData
)
\param messageSeverity: same as createInfo
\param messageType: same as createInfo
VkDebugUtilsMessengerCallbackDataEXT
   */
  createInfo.pfnUserCallback = debugCallback;
}


/**
  Print validation layer output

  A static function that prints the output of the
  validation layer. The signature of the function fits to
  the @see populateDebugMessengerCreateInfo()
  content.
 */
/**
Set @see debugMessenger up by populating its related
info.
*/
void HelloTriangle::setupDebugMessenger() {
  if (!enableValidationLayers)
    return;
  //
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);

  CHECK_VK2(CreateDebugUtilsMessengerExt(instance, &createInfo, nullptr,
                                         &debugMessenger),
            "failed to create and setup debug messenger");
}
VkSurfaceFormatKHR HelloTriangle::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availables) {
  //
  for (const auto &aformat : availables) {
    if (aformat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        aformat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      // ACES - XYZ
      return aformat;
    }
  }
  return availables[0];
}
/**
 Choose right present mode for swapchain

 Available present modes: VK_PRESENT_MODE_*_KHR
 - IMMEDIATE :  direct rendering of image to screen
 - FIFO: Render from the front of queue and put it back of
 the queue
 - FIFO_RELAXED: Variation on FIFO. Instead of waiting for
 an empty queue,
 we render the image right away.
 - MAILBOX: variation of FIFO. When the queue is full
 replace the images
 with newer ones.
 */
VkPresentModeKHR HelloTriangle::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availables) {
  //
  for (const auto &apresent : availables) {
    if (apresent == VK_PRESENT_MODE_MAILBOX_KHR) {
      return apresent;
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
HelloTriangle::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
  //
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    VkExtent2D actual_extent = {static_cast<uint32_t>(width),
                                static_cast<uint32_t>(height)};
    actual_extent.width = std::max(
        capabilities.minImageExtent.width,
        std::min(capabilities.minImageExtent.width, actual_extent.width));
    actual_extent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.minImageExtent.height, actual_extent.height));
    return actual_extent;
  }
}

/**
  Check if device support requested extensions.

  As usual we query the device for available extensions
  then check whether what we had requested is available
  by erasing from requested extensions.
 */
bool HelloTriangle::checkDeviceExtensionSupport(VkPhysicalDevice pdev) {
  //
  uint32_t ext_count = 0;
  vkEnumerateDeviceExtensionProperties(pdev, nullptr, &ext_count, nullptr);

  std::vector<VkExtensionProperties> available_exts(ext_count);

  vkEnumerateDeviceExtensionProperties(pdev, nullptr, &ext_count,
                                       available_exts.data());

  std::set<std::string> requested_extensions(device_extensions.begin(),
                                             device_extensions.end());

  for (const auto &ext : available_exts) {
    requested_extensions.erase(ext.extensionName);
  }
  return requested_extensions.empty();
}
VkShaderModule
HelloTriangle::createShaderModule(const std::vector<char> &shaderCode) {
  //
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = shaderCode.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());

  VkShaderModule shaderModule;
  CHECK_VK2(vkCreateShaderModule(logical_dev.device(), &createInfo, nullptr,
                                 &shaderModule),
            "failed to create shader module");
  return shaderModule;
}
void HelloTriangle::createFramebuffers() {
  swapchain_framebuffers.resize(swap_chain.view_size());
  for (std::size_t i = 0; i < swap_chain.view_size(); i++) {
    // vk image view per frame
    std::vector<VkImageView> image_attachments = {swap_chain.simage_views[i],
                                                  depth_image_view};
    auto width = swap_chain.sextent.width;
    auto height = swap_chain.sextent.height;
    auto layer_nb = 1;
    auto vbuffer = vulkan_buffer<VkFramebuffer>(
        width, height, layer_nb, render_pass, image_attachments, logical_dev);
    swapchain_framebuffers[i] = vbuffer;
    //
  }
}
void HelloTriangle::createDepthRessources() {
  VkFormat depth_format = findDepthFormat();
  auto width = swap_chain.sextent.width;
  auto height = swap_chain.sextent.height;
  auto tiling = VK_IMAGE_TILING_OPTIMAL;
  auto usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  auto memflag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  createImage(width, height, depth_format, tiling, usage, memflag, depth_image,
              depth_image_memory);
  depth_image_view =
      createImageView(depth_image, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT);
  // transitionImageLayout(
  //    depth_image, depth_format,
  //    VK_IMAGE_LAYOUT_UNDEFINED,
  //    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}
VkFormat
HelloTriangle::findSupportedFormat(const std::vector<VkFormat> &candidates,
                                   VkImageTiling tiling,
                                   VkFormatFeatureFlags features) {
  //
  VkFormat c = VK_FORMAT_UNDEFINED;
  for (VkFormat candidate : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physical_dev.device(), candidate,
                                        &props);
    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      c = candidate;
      return c;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      c = candidate;
      return c;
    } else {
      throw std::runtime_error("failed to find supported depth format");
    }
  }
  if (c == VK_FORMAT_UNDEFINED) {
    throw std::runtime_error("failed to find supported depth format");
  }
  return c;
}
VkFormat HelloTriangle::findDepthFormat() {
  auto candidates = {
      VK_FORMAT_D32_SFLOAT,
      VK_FORMAT_D32_SFLOAT_S8_UINT,
      VK_FORMAT_D24_UNORM_S8_UINT,
  };
  auto tiling = VK_IMAGE_TILING_OPTIMAL;
  auto features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
  return findSupportedFormat(candidates, tiling, features);
}
bool HelloTriangle::hasStencilSupport(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
         format == VK_FORMAT_D24_UNORM_S8_UINT;
}
void HelloTriangle::createTextureImage() {
  //
  int imwidth, imheight, imchannel;
  const char *mpath = model_texture_path.c_str();
  stbi_uc *pixels =
      stbi_load(mpath, &imwidth, &imheight, &imchannel, STBI_rgb_alpha);
  VkDeviceSize imsize = imwidth * imheight * 4;
  if (!pixels) {
    throw std::runtime_error("pixel data can not be loaded");
  }
  VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  VkMemoryPropertyFlags mem_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  createBuffer(imsize, usage, mem_flags, staging_buffer, stage_buffer_memory);
  void *data;
  vkMapMemory(logical_dev.device(), stage_buffer_memory, 0, imsize, 0, &data);
  memcpy(data, pixels, static_cast<std::size_t>(imsize));
  vkUnmapMemory(logical_dev.device(), stage_buffer_memory);
  //
  stbi_image_free(pixels);

  // create texture image as vulkan image
  VkFormat imformat = VK_FORMAT_R8G8B8A8_SRGB;
  VkImageTiling imtiling = VK_IMAGE_TILING_OPTIMAL;
  VkImageUsageFlags imusage =
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  VkMemoryPropertyFlags improps = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  createImage(imwidth, imheight, imformat, imtiling, imusage, improps,
              texture_image, texture_image_memory);
  //
  auto format = VK_FORMAT_R8G8B8A8_SRGB;
  auto old_layout = VK_IMAGE_LAYOUT_UNDEFINED;
  auto new_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  transitionImageLayout(texture_image, format, old_layout, new_layout);
  copyBufferToImage(staging_buffer, texture_image,
                    static_cast<uint32_t>(imwidth),
                    static_cast<uint32_t>(imheight));

  old_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  new_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  transitionImageLayout(texture_image, format, old_layout, new_layout);

  vkDestroyBuffer(logical_dev.device(), staging_buffer, nullptr);
  vkFreeMemory(logical_dev.device(), stage_buffer_memory, nullptr);
}
void HelloTriangle::createImage(uint32_t imw, uint32_t imh, VkFormat format,
                                VkImageTiling tiling, VkImageUsageFlags imusage,
                                VkMemoryPropertyFlags improps, VkImage &vimage,
                                VkDeviceMemory &vimage_memory) {
  VkImageCreateInfo img_info{};
  img_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  img_info.imageType = VK_IMAGE_TYPE_2D;
  img_info.extent.width = imw;
  img_info.extent.height = imh;
  img_info.extent.depth = 1;
  img_info.mipLevels = 1;
  img_info.arrayLayers = 1;
  img_info.format = format;
  img_info.tiling = tiling;
  img_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  img_info.usage = imusage;
  img_info.samples = VK_SAMPLE_COUNT_1_BIT;
  img_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  CHECK_VK2(vkCreateImage(logical_dev.device(), &img_info, nullptr, &vimage),
            "failed to create image");
  //
  VkMemoryRequirements mem_req;
  vkGetImageMemoryRequirements(logical_dev.device(), vimage, &mem_req);

  //
  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = mem_req.size;
  allocInfo.memoryTypeIndex = findMemoryType(mem_req.memoryTypeBits, improps);

  CHECK_VK2(vkAllocateMemory(logical_dev.device(), &allocInfo, nullptr,
                             &vimage_memory),
            "failed to create image memory");
  vkBindImageMemory(logical_dev.device(), vimage, vimage_memory, 0);
}
void HelloTriangle::transitionImageLayout(VkImage image, VkFormat format,
                                          VkImageLayout old_layout,
                                          VkImageLayout new_layout) {
  VkCommandBuffer command_buffer = beginSignalCommand();
  //
  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = old_layout;
  barrier.newLayout = new_layout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags source_stage;
  VkPipelineStageFlags dst_stage;
  //
  if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (hasStencilSupport(format)) {
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
  } else {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  // pipeline 1
  if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
      new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {

    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    // pipeline 2
  } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    // pipeline 3
  } else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
             new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.srcAccessMask = 0;
    auto rwflag = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    barrier.dstAccessMask = rwflag;
    source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dst_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  } else {
    throw std::invalid_argument("unsupported layout transition");
  }
  vkCmdPipelineBarrier(command_buffer, source_stage, dst_stage, 0, 0, nullptr,
                       0, nullptr, 1, &barrier);
  endSignalCommand(command_buffer);
}

void HelloTriangle::copyBufferToImage(VkBuffer buffer, VkImage image,
                                      uint32_t width, uint32_t height) {
  VkCommandBuffer cbuffer = beginSignalCommand();

  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  vkCmdCopyBufferToImage(cbuffer, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  endSignalCommand(cbuffer);
}
VkImageView HelloTriangle::createImageView(VkImage image, VkFormat image_format,
                                           VkImageAspectFlags aspect_flags) {
  VkImageViewCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createInfo.image = image;
  createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  createInfo.format = image_format;
  createInfo.subresourceRange.aspectMask = aspect_flags;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;
  VkImageView imview;
  CHECK_VK2(
      vkCreateImageView(logical_dev.device(), &createInfo, nullptr, &imview),
      "failed to create texture image view");
  return imview;
}
void HelloTriangle::createTextureImageView() {
  //
  auto imformat = VK_FORMAT_R8G8B8A8_SRGB;
  texture_image_view =
      createImageView(texture_image, imformat, VK_IMAGE_ASPECT_COLOR_BIT);
}
void HelloTriangle::createTextureSampler() {
  VkPhysicalDeviceProperties props{};
  vkGetPhysicalDeviceProperties(physical_dev.device(), &props);

  // sampler create info
  VkSamplerCreateInfo cinfo{};
  cinfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  cinfo.magFilter = VK_FILTER_LINEAR;
  cinfo.minFilter = VK_FILTER_LINEAR;

  // how should sampler behave if the texture image does not
  // cover all of the visible surface:
  cinfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  cinfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  cinfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

  //
  cinfo.anisotropyEnable = VK_TRUE;
  cinfo.maxAnisotropy = props.limits.maxSamplerAnisotropy;
  cinfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  cinfo.unnormalizedCoordinates = VK_FALSE;
  cinfo.compareEnable = VK_FALSE;
  cinfo.compareOp = VK_COMPARE_OP_ALWAYS;
  cinfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

  // create sampler with given information
  CHECK_VK2(
      vkCreateSampler(logical_dev.device(), &cinfo, nullptr, &texture_sampler),
      "failed to create texture sampler");
}
VkCommandBuffer HelloTriangle::beginSignalCommand() {
  //
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = command_pool.pool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer cbuffer;
  vkAllocateCommandBuffers(logical_dev.device(), &allocInfo, &cbuffer);

  VkCommandBufferBeginInfo binfo{};
  binfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  binfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  //
  vkBeginCommandBuffer(cbuffer, &binfo);
  return cbuffer;
}
void HelloTriangle::endSignalCommand(VkCommandBuffer cbuffer) {
  vkEndCommandBuffer(cbuffer);

  VkSubmitInfo sinfo{};
  sinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  sinfo.commandBufferCount = 1;
  sinfo.pCommandBuffers = &cbuffer;

  //
  vkQueueSubmit(logical_dev.graphics_queue, 1, &sinfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(logical_dev.graphics_queue);

  vkFreeCommandBuffers(logical_dev.device(), command_pool.pool, 1, &cbuffer);
}
void HelloTriangle::loadModel() {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;
  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                        model_path.c_str())) {
    throw std::runtime_error(warn + err);
  }
  std::unordered_map<Vertex, std::uint32_t> uVertices{};
  for (const auto &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {
      Vertex v{};
      auto stride = 3;
      auto vindex = index.vertex_index;
      v.pos = {attrib.vertices[stride * vindex + 0],
               attrib.vertices[stride * vindex + 1],
               attrib.vertices[stride * vindex + 2]};
      auto tex_stride = 2;
      auto tindex = index.texcoord_index;
      v.texCoord = {attrib.texcoords[tex_stride * tindex + 0],
                    1.0f - attrib.texcoords[tex_stride * tindex + 1]};
      v.color = {1.0f, 1.0f, 1.0f};

      if (uVertices.count(v) == 0) {
        uVertices[v] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(v);
      }
      indices.push_back(uVertices[v]);
    }
  }
}
uint32_t HelloTriangle::findMemoryType(uint32_t filter,
                                       VkMemoryPropertyFlags flags) {
  //
  VkPhysicalDeviceMemoryProperties memProps;
  vkGetPhysicalDeviceMemoryProperties(physical_dev.device(), &memProps);
  for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
    if ((filter & (1 << i)) &&
        (memProps.memoryTypes[i].propertyFlags & flags) == flags) {
      return i;
    }
  }
  throw std::runtime_error("could not find a suitable memory type");
}
void HelloTriangle::createSyncObjects() {
  image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
  render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
  current_fences.resize(MAX_FRAMES_IN_FLIGHT);
  images_in_flight.resize(swap_chain.simages.size(), VK_NULL_HANDLE);

  // create semaphore info
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  // create fences
  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  auto mxflight = static_cast<unsigned int>(MAX_FRAMES_IN_FLIGHT);
  for (std::size_t i = 0; i < mxflight; i++) {
    CHECK_VK2(vkCreateSemaphore(logical_dev.device(), &semaphoreInfo, nullptr,
                                &image_available_semaphores[i]),
              "Failed to create image available semaphore");
    CHECK_VK2(vkCreateSemaphore(logical_dev.device(), &semaphoreInfo, nullptr,
                                &render_finished_semaphores[i]),
              "Failed to create render finished semaphore");

    CHECK_VK2(vkCreateFence(logical_dev.device(), &fenceInfo, nullptr,
                            &current_fences[i]),
              "Failed to in flight fence");
  }
}
void HelloTriangle::recreateSwapchain() {
  //
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(window, &width, &height);
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(logical_dev.device());
  auto vs = cmd_buffers.to_vec();
  swap_chain.destroy(logical_dev, command_pool.pool, vs, swapchain_framebuffers,
                     render_pass, graphics_pipeline, pipeline_layout,
                     uniform_buffers, uniform_buffer_memories, descriptor_pool,
                     depth_image, depth_image_view, depth_image_memory);
  swap_chain = swapchain(physical_dev, logical_dev, window);
  // 1. render pass
  createRenderPass();
  // 2. graphics pipeline
  createGraphicsPipeline();

  // 3. create depth ressources
  createDepthRessources();
  // 3. frame buffer
  createFramebuffers();
  // 4. uniform buffer
  createUniformBuffer();
  // 5. descriptor pool
  createDescriptorPool();
  // 6. descriptor pool
  createDescriptorSets();
  // 7. command buffers
  createCommandBuffers();
}
void HelloTriangle::updateUniformBuffer(uint32_t image_index) {
  UniformBufferObject ubo;
  glm::mat4 model(1.0f);
  ubo.model =
      glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::vec3 cam_pos(2.0f);
  glm::vec3 cam_target(0.0f);
  glm::vec3 world_up(0.0f, 0.0f, 1.0f);
  ubo.view = glm::lookAt(cam_pos, cam_target, world_up);
  float aspect_ratio =
      swap_chain.sextent.width / static_cast<float>(swap_chain.sextent.height);
  float near_plane_distance = 0.1f;
  float far_plane_distance = 100.0f;
  ubo.proj = glm::perspective(glm::radians(45.0f), aspect_ratio,
                              near_plane_distance, far_plane_distance);
  ubo.proj[1][1] *= -1;

  void *data;
  vkMapMemory(logical_dev.device(), uniform_buffer_memories[image_index], 0,
              sizeof(ubo), 0, &data);
  memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(logical_dev.device(), uniform_buffer_memories[image_index]);
}
} // namespace vtuto
