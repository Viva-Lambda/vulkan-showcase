#pragma once
#include <commandbuffer.hpp>
#include <cstdint>
#include <debug.hpp>
#include <external.hpp>
#include <framebuffer.hpp>
#include <imageview.hpp>
#include <ldevice.hpp>
#include <pdevice.hpp>
#include <support.hpp>
#include <swapchain.hpp>
#include <triangle.hpp>
#include <utils.hpp>
#include <vertex.hpp>

using namespace vtuto;

namespace vtuto {

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

//
const std::string model_path = "./assets/models/viking.obj";
const std::string model_texture_path = "./assets/models/viking.png";

class HelloTriangle {
public:
  std::string win_title = "Vulkan Window";
  uint32_t win_width = WIDTH;
  uint32_t win_height = HEIGHT;

  /** instance of the vulkan application */
  VkInstance instance;

  /** debug callback function handler */
  VkDebugUtilsMessengerEXT debugMessenger;

  /** physical device pointer*/
  vulkan_device<VkPhysicalDevice> physical_dev;

  /** logical device pointer */
  vulkan_device<VkDevice> logical_dev;

  /** swapchain for handling frame rate*/
  swapchain swap_chain;

  /** swap chain frame buffers*/
  std::vector<vulkan_buffer<VkFramebuffer>> swapchain_framebuffers;

  /** render pass */
  VkRenderPass render_pass;

  /** descriptor set layout*/
  VkDescriptorSetLayout descriptor_set_layout;

  /** descriptor pool*/
  VkDescriptorPool descriptor_pool;

  std::vector<VkDescriptorSet> descriptor_sets;

  /** graphics pipeline layout*/
  VkPipelineLayout pipeline_layout;

  /** graphics pipeline object*/
  VkPipeline graphics_pipeline;

  /** command pool for command buffer*/
  vk_command_pool command_pool;
  vulkan_buffers<VkCommandBuffer> cmd_buffers;

  /** texture staging buffer */
  VkBuffer staging_buffer;
  VkDeviceMemory stage_buffer_memory;
  VkImage texture_image;
  VkDeviceMemory texture_image_memory;

  /** texture image view */
  VkImageView texture_image_view;

  /** texture sampler */
  VkSampler texture_sampler;

  /** depth image related*/
  VkImage depth_image;
  VkImageView depth_image_view;
  VkDeviceMemory depth_image_memory;

  /** vertices per scene and indices per scene*/
  std::vector<Vertex> vertices;
  std::vector<std::uint32_t> indices;

  /** vertex buffer*/
  VkBuffer vertex_buffer;
  VkDeviceMemory vertex_buffer_memory;

  /** index buffer*/
  VkBuffer index_buffer;
  VkDeviceMemory index_buffer_memory;

  /** uniform buffer*/
  std::vector<VkBuffer> uniform_buffers;
  std::vector<VkDeviceMemory> uniform_buffer_memories;

  /** vk semaphore to hold available and rendered images */
  std::vector<VkSemaphore> image_available_semaphores;
  std::vector<VkSemaphore> render_finished_semaphores;

  /** fence image for drawing */
  std::vector<VkFence> current_fences;
  std::vector<VkFence> images_in_flight;
  std::size_t current_frame = 0;

  /** maximum frames in flight*/
  const int MAX_FRAMES_IN_FLIGHT = 2;

  /** check framebuffer state*/
  bool framebuffer_resized = false;

public:
  HelloTriangle() {}
  HelloTriangle(std::string wTitle, const uint32_t &w, const uint32_t &h)
      : win_title(wTitle), win_width(w), win_height(h) {}
  /**
    Run application.

    Steps to run the application
   */
  void run();

private:
  GLFWwindow *window; // window for visualizing object.

  /**
    Initialize window.

    Gives window hints. Sets its size, its title, etc.
   */
  void initWindow();
  static void framebuffer_resize_callback(GLFWwindow *win, int w, int h) {
    //
    auto app = reinterpret_cast<HelloTriangle *>(glfwGetWindowUserPointer(win));
    app->framebuffer_resized = true;
  }
  /**
    Initialize vulkan.

    Steps to initialize a vulkan api
    1. Create a vulkan instance
   */
  void initVulkan();
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
  void createInstance();
  /**
    Rendering loop.

    Render elements to window. Acquire user input
   */
  void renderLoop();
  /**
    Clean up ressources.

    Destroy window, and other ressources.
   */
  void cleanUp();
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
  bool checkValidationLayerSupport();
  /**
    Specify properties of the debug messenger callback

    We add its type, requested message severities, message
    types, and we add which debug callback function is going
    to be used
   */
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);

  void createSurface();

  void createRenderPass();

  /**
    Print validation layer output

    A static function that prints the output of the
    validation layer. The signature of the function fits to
    the @see populateDebugMessengerCreateInfo()
    content.
   */
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                void *pUserData) {
    std::cerr << "validation layer output: " << pCallbackData->pMessage
              << std::endl;
    return VK_FALSE;
  }
  /**
    Set @see debugMessenger up by populating its related
    info.
   */
  void setupDebugMessenger();
  std::vector<const char *> getRequiredExtensions();
  /**
Find device family indices for given VkPhysicalDevice

We query the given physical device for physical device
family properties. We break away if the device has
complete
number of indices for given device family.
*/
  QueuFamilyIndices find_family_indices(VkPhysicalDevice pdev);
  void pickPhysicalDevice();
  VkSurfaceFormatKHR
  chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availables);
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
  VkPresentModeKHR
  chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availables);

  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
  /**
    Query supported swap chain details.

    \c vkGetPhysicalDeviceSurfaceFormatsKHR() \c for
    querying supported
    color formats (unorm, srgb, etc)

    \c vkGetPhysicalDeviceSurfacePresentModesKHR \c for
    querying supported
    data structures for presentation to be used in swap
    chain management.
   */
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pdev);
  /**
    Check if the device is suitable for implementing a swap
    chain
   */
  bool is_device_suitable(VkPhysicalDevice pdev);
  /**
    Check if device support requested extensions.

    As usual we query the device for available extensions
    then check whether what we had requested is available
    by erasing from requested extensions.
   */
  bool checkDeviceExtensionSupport(VkPhysicalDevice pdev);
  void createPhysicalDevice();
  void createLogicalDevice();
  VkShaderModule createShaderModule(const std::vector<char> &shaderCode);
  void createGraphicsPipeline();
  void createDescriptorSetLayout();
  void createDescriptorPool();
  void createDescriptorSets();
  void createFramebuffers();
  uint32_t findMemoryType(uint32_t filter, VkMemoryPropertyFlags flags);
  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat findDepthFormat();
  bool hasStencilSupport(VkFormat format);
  void loadModel();
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffer();
  void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags mem_flags, VkBuffer &buffer,
                    VkDeviceMemory &buffer_memory);
  void createCommandPool();
  void createCommandBuffers();
  void createSyncObjects();
  void recreateSwapchain();
  void createDepthRessources();
  void createTextureImage();
  void createTextureSampler();
  VkImageView createImageView(VkImage image, VkFormat image_format,
                              VkImageAspectFlags aspect_flags);
  void createTextureImageView();
  void createImage(uint32_t imw, uint32_t imh, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags imusage,
                   VkMemoryPropertyFlags improps, VkImage &vimage,
                   VkDeviceMemory &vimage_memory);
  void updateUniformBuffer(uint32_t image_index);
  void draw();
  VkCommandBuffer beginSignalCommand();
  void endSignalCommand(VkCommandBuffer cbuffer);
  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout old_layout,
                             VkImageLayout new_layout);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height);
};
} // namespace vtuto
