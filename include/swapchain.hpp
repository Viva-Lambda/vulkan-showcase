// swapchain for vulkan instance
#pragma once

#include <debug.hpp>
#include <external.hpp>
#include <framebuffer.hpp>
#include <imageview.hpp>
#include <ldevice.hpp>
#include <pdevice.hpp>
#include <support.hpp>
#include <utils.hpp>
#include <vkutils/varia.hpp>

using namespace vtuto;

namespace vtuto {
//
class swapchain {
public:
  /** swapchain for handling frame rate*/
  VkSwapchainKHR chain;

  /** images in swap chain */
  std::vector<VkImage> simages;

  /** swapchain image format*/
  VkFormat simage_format;

  /** swapchain extent*/
  VkExtent2D sextent;

  /** swapchain image view */
  image_views simage_views;

public:
  swapchain() {}
  swapchain(
      const vulkan_device<VkPhysicalDevice> &physical_dev,
      vulkan_device<VkDevice> logical_dev,
      GLFWwindow *window,
      unsigned int image_arr_layers = 1) {
    SwapChainSupportDetails swap_details =
        SwapChainSupportDetails::querySwapChainSupport(
            physical_dev.pdevice, physical_dev.surface);

    VkSurfaceFormatKHR surfaceFormat =
        chooseSwapSurfaceFormat(swap_details.formats);

    VkPresentModeKHR presentMode =
        chooseSwapPresentMode(swap_details.present_modes);

    VkExtent2D extent =
        chooseSwapExtent(swap_details.capabilities, window);

    uint32_t img_count =
        swap_details.capabilities.minImageCount + 1;
    if (swap_details.capabilities.maxImageCount > 0 &&
        img_count >
            swap_details.capabilities.maxImageCount) {
      img_count = swap_details.capabilities.maxImageCount;
    }

    // Swapchain info details
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    // surface
    createInfo.surface = physical_dev.surface;

    // image type, size etc
    createInfo.minImageCount = img_count;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = image_arr_layers;
    createInfo.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueuFamilyIndices indices =
        QueuFamilyIndices::find_family_indices(
            physical_dev.pdevice, physical_dev.surface);
    uint32_t qfamily_indices[] = {
        indices.graphics_family.value(),
        indices.present_family.value()};

    if (indices.graphics_family != indices.present_family) {
      createInfo.imageSharingMode =
          VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = qfamily_indices;
    } else {
      createInfo.imageSharingMode =
          VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform =
        swap_details.capabilities.currentTransform;
    createInfo.compositeAlpha =
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    // handling of used ressources
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    //
    CHECK_VK2(vkCreateSwapchainKHR(logical_dev.device(),
                                   &createInfo, nullptr,
                                   &chain),
              "failed to create a swap chain");

    CHECK_VK2(vkGetSwapchainImagesKHR(logical_dev.device(),
                                      chain, &img_count,
                                      nullptr),
              "failed to reserve for swapchain images");
    simages.resize(img_count);
    CHECK_VK2(vkGetSwapchainImagesKHR(logical_dev.device(),
                                      chain, &img_count,
                                      simages.data()),
              "failed to set swapchain images");
    simage_format = surfaceFormat.format;
    sextent = extent;
    set_image_views(logical_dev);
  }
  void
  set_image_views(vulkan_device<VkDevice> logical_dev) {
    simage_views =
        image_views(simages, simage_format, logical_dev);
  }
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availables) {
    //
    for (const auto &aformat : availables) {
      if (aformat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          aformat.colorSpace ==
              VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        // ACES - XYZ
        return aformat;
      }
    }
    return availables[0];
  }
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availables) {
    //
    for (const auto &apresent : availables) {
      if (apresent == VK_PRESENT_MODE_MAILBOX_KHR) {
        return apresent;
      }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
  }
  VkExtent2D chooseSwapExtent(
      const VkSurfaceCapabilitiesKHR &capabilities,
      GLFWwindow *window) {
    //
    if (capabilities.currentExtent.width != UINT32_MAX) {
      return capabilities.currentExtent;
    } else {
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      VkExtent2D actual_extent = {
          static_cast<uint32_t>(width),
          static_cast<uint32_t>(height)};
      actual_extent.width = std::max(
          capabilities.minImageExtent.width,
          std::min(capabilities.minImageExtent.width,
                   actual_extent.width));
      actual_extent.height = std::max(
          capabilities.minImageExtent.height,
          std::min(capabilities.minImageExtent.height,
                   actual_extent.height));
      return actual_extent;
    }
  }
  std::size_t view_size() { return simage_views.size(); }
  void destroy(
      vulkan_device<VkDevice> &logical_dev,
      VkCommandPool &command_pool,
      std::vector<VkCommandBuffer> &command_buffers,
      std::vector<vulkan_buffer<VkFramebuffer>>
          &swapchain_framebuffers,
      VkRenderPass &render_pass,
      VkPipeline &graphics_pipeline,
      VkPipelineLayout &pipeline_layout,
      std::vector<VkBuffer> &uniform_buffers,
      std::vector<VkDeviceMemory> &uniform_buffer_memories,
      VkDescriptorPool &descriptor_pool,
      VkImage &depth_image, VkImageView &depth_image_view,
      VkDeviceMemory &depth_image_memory) {
    // clear out depth image view
    vkDestroyImageView(logical_dev.device(),
                       depth_image_view, nullptr);
    vkDestroyImage(logical_dev.device(), depth_image,
                   nullptr);
    vkFreeMemory(logical_dev.device(), depth_image_memory,
                 nullptr);
    //
    vkFreeCommandBuffers(
        logical_dev.device(), command_pool,
        static_cast<uint32_t>(command_buffers.size()),
        command_buffers.data());

    for (auto &framebuffer : swapchain_framebuffers) {
      //
      framebuffer.destroy(logical_dev);
    }
    vkDestroyPipeline(logical_dev.device(),
                      graphics_pipeline, nullptr);
    // 1. destroy pipeline layout
    vkDestroyPipelineLayout(logical_dev.device(),
                            pipeline_layout, nullptr);
    // 2. destroy rendering pass
    vkDestroyRenderPass(logical_dev.device(), render_pass,
                        nullptr);
    // 2. destroy swap chain image views
    simage_views.destroy(logical_dev);
    // 3. destroy swap chain
    vkDestroySwapchainKHR(logical_dev.device(), chain,
                          nullptr);
    // 4. destroy uniform buffers
    for (std::size_t i = 0; i < simages.size(); i++) {
      vkDestroyBuffer(logical_dev.device(),
                      uniform_buffers[i], nullptr);
      vkFreeMemory(logical_dev.device(),
                   uniform_buffer_memories[i], nullptr);
    }
    // 5. destroy descriptor pool
    vkDestroyDescriptorPool(logical_dev.device(),
                            descriptor_pool, nullptr);
  }
};
}
