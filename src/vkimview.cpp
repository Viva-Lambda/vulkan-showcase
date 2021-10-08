#include <hellotriangle.hpp>
using namespace vtuto;

namespace vtuto {

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
} // namespace vtuto
