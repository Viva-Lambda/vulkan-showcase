// image view in vulkan app
#pragma once
#include <debug.hpp>
#include <external.hpp>
#include <ldevice.hpp>
#include <pdevice.hpp>
#include <support.hpp>
#include <utils.hpp>

using namespace vtuto;

namespace vtuto {
//
class image_view {
public:
  VkImageView view;

public:
  image_view() {}
  image_view(
      const VkImage &image, VkFormat image_format,
      vulkan_device<VkDevice> &logical_dev,
      VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D,
      uint base_mipmap_index = 0, uint mipmap_count = 1,
      uint array_layer_index = 0,
      uint array_layer_count = 1,
      VkComponentSwizzle r_swizzle =
          VK_COMPONENT_SWIZZLE_IDENTITY,
      VkComponentSwizzle g_swizzle =
          VK_COMPONENT_SWIZZLE_IDENTITY,
      VkComponentSwizzle b_swizzle =
          VK_COMPONENT_SWIZZLE_IDENTITY,
      VkComponentSwizzle a_swizzle =
          VK_COMPONENT_SWIZZLE_IDENTITY,
      VkImageAspectFlagBits aspect_mask =
          VK_IMAGE_ASPECT_COLOR_BIT) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    /** view type can be 1d texture, 2d texture, 3d
     * textures and cubemaps*/
    createInfo.viewType = view_type;
    createInfo.format = image_format;

    /** vec.xx == vec2(vec.x, vec.x) */
    createInfo.components.r = r_swizzle;
    createInfo.components.g = g_swizzle;
    createInfo.components.b = b_swizzle;
    createInfo.components.a = a_swizzle;

    createInfo.subresourceRange.aspectMask = aspect_mask;
    createInfo.subresourceRange.baseMipLevel =
        base_mipmap_index;
    createInfo.subresourceRange.levelCount = mipmap_count;
    createInfo.subresourceRange.baseArrayLayer =
        array_layer_index;
    createInfo.subresourceRange.layerCount =
        array_layer_count;
    CHECK_VK2(vkCreateImageView(logical_dev.device(),
                               &createInfo, nullptr, &view),
             "failed to create image view");
  }
  image_view(VkImageViewCreateInfo createInfo,
             vulkan_device<VkDevice> &logical_dev) {
    CHECK_VK2(vkCreateImageView(logical_dev.device(),
                               &createInfo, nullptr, &view),
             "failed to create image view");
  }
  void destroy(vulkan_device<VkDevice> &logical_dev) {
    vkDestroyImageView(logical_dev.device(), view, nullptr);
  }
};
class image_views {
public:
  std::vector<image_view> views;

public:
  image_views() {}
  image_views(
      const std::vector<VkImage> &images,
      VkFormat image_format,
      vulkan_device<VkDevice> &logical_dev,
      VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D,
      uint base_mipmap_index = 0, uint mipmap_count = 1,
      uint array_layer_index = 0,
      uint array_layer_count = 1,
      VkComponentSwizzle r_swizzle =
          VK_COMPONENT_SWIZZLE_IDENTITY,
      VkComponentSwizzle g_swizzle =
          VK_COMPONENT_SWIZZLE_IDENTITY,
      VkComponentSwizzle b_swizzle =
          VK_COMPONENT_SWIZZLE_IDENTITY,
      VkComponentSwizzle a_swizzle =
          VK_COMPONENT_SWIZZLE_IDENTITY,
      VkImageAspectFlagBits aspect_mask =
          VK_IMAGE_ASPECT_COLOR_BIT) {
    views.resize(images.size());
    for (std::size_t i = 0; i < images.size(); i++) {
      views[i] = image_view(
          images[i], image_format, logical_dev, view_type,
          base_mipmap_index, mipmap_count,
          array_layer_index, array_layer_count, r_swizzle,
          g_swizzle, b_swizzle, a_swizzle, aspect_mask);
    }
  }
  image_views(const std::vector<VkImage> &images,
              VkImageViewCreateInfo createInfo,
              vulkan_device<VkDevice> &logical_dev) {
    views.resize(images.size());
    for (std::size_t i = 0; i < images.size(); i++) {
      VkImageViewCreateInfo iminfo = createInfo;
      iminfo.image = images[i];
      views[i] = image_view(iminfo, logical_dev);
    }
  }
  image_views(
      const std::vector<VkImageViewCreateInfo> infos,
      vulkan_device<VkDevice> &logical_dev) {
    views.resize(infos.size());
    for (std::size_t i = 0; i < infos.size(); i++) {
      VkImageViewCreateInfo iminfo = infos[i];
      views[i] = image_view(iminfo, logical_dev);
    }
  }
  void destroy(vulkan_device<VkDevice> &logical_dev) {
    for (auto &view : views) {
      view.destroy(logical_dev);
    }
  }

  std::size_t size() { return views.size(); }
  VkImageView operator[](std::size_t i) {
    return views[i].view;
  }
  std::vector<VkImageView> data() {
    std::vector<VkImageView> vs;
    vs.resize(views.size());
    for (uint i = 0; i < views.size(); i++) {
      vs[i] = views[i].view;
    }
    return vs;
  }
};
}
