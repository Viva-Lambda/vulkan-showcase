// vulkan frame buffer object
#pragma once
#include <external.hpp>
#include <imageview.hpp>
#include <support.hpp>
#include <vbuffer.hpp>

using namespace vtuto;

namespace vtuto {
template <> class vulkan_buffer<VkFramebuffer> {
public:
  VkFramebuffer buffer;

public:
  vulkan_buffer() {}
  vulkan_buffer(uint32_t width, uint32_t height,
                uint32_t layer_nb,
                const VkRenderPass &render_pass,
                const std::vector<VkImageView> &imviews,
                vulkan_device<VkDevice> &logical_dev) {
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType =
        VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = render_pass;
    framebufferInfo.attachmentCount =
        static_cast<uint32_t>(imviews.size());
    framebufferInfo.pAttachments = imviews.data();
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = layer_nb;

    CHECK_VK2(vkCreateFramebuffer(logical_dev.device(),
                                 &framebufferInfo, nullptr,
                                 &buffer),
             "failed to create framebuffer for image view");
  }
  vulkan_buffer(VkFramebufferCreateInfo info,
                vulkan_device<VkDevice> &logical_dev) {
    CHECK_VK2(vkCreateFramebuffer(logical_dev.device(),
                                 &info, nullptr, &buffer),
             "failed to create framebuffer for image view");
  }
  void destroy(vulkan_device<VkDevice> &logical_dev) {
    vkDestroyFramebuffer(logical_dev.device(), buffer,
                         nullptr);
  }
};
}
