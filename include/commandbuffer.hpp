// command buffer for vulkan application
#pragma once
//
#include <external.hpp>
#include <framebuffer.hpp>
#include <ldevice.hpp>
#include <pdevice.hpp>
#include <support.hpp>
#include <triangle.hpp>
#include <utils.hpp>
#include <vbuffer.hpp>

using namespace vtuto;
namespace vtuto {
struct VkDrawInfo {
  uint32_t vertex_count;
  uint32_t instance_count;
  uint32_t first_vertex_index;
  uint32_t first_instance_index;
};
class vk_command_pool {
public:
  VkCommandPool pool;

public:
  vk_command_pool() {}
  vk_command_pool(
      const vulkan_device<VkPhysicalDevice> &physical_dev,
      vulkan_device<VkDevice> &logical_dev) {
    QueuFamilyIndices qfi =
        QueuFamilyIndices::find_family_indices(
            physical_dev.pdevice, physical_dev.surface);
    VkCommandPoolCreateInfo commandPoolInfo{};
    commandPoolInfo.sType =
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.queueFamilyIndex =
        qfi.graphics_family.value();
    CHECK_VK(vkCreateCommandPool(logical_dev.device(),
                                 &commandPoolInfo, nullptr,
                                 &pool),
             "failed to create command pool");
  }
  void destroy(vulkan_device<VkDevice> &logical_dev) {
    vkDestroyCommandPool(logical_dev.device(), pool,
                         nullptr);
  }
};
template <> class vulkan_buffer<VkCommandBuffer> {
  //
public:
  VkCommandBuffer buffer;

public:
  vulkan_buffer() {}
  vulkan_buffer(
      VkCommandBuffer loc,
      vulkan_buffer<VkFramebuffer> &sc_framebuffer,
      VkRenderPass &render_pass,
      VkExtent2D swap_chain_extent,
      VkPipeline graphics_pipeline, VkBuffer vertex_buffer,
      VkBuffer index_buffer, std::vector<uint32_t> indices,
      VkDescriptorSet descriptor_set,
      VkPipelineLayout pipeline_layout,
      int32_t render_offset_x = 0,
      int32_t render_offset_y = 0,
      VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f},
      uint clearValueCount = 1,
      VkSubpassContents subpass_contents =
          VK_SUBPASS_CONTENTS_INLINE,
      VkPipelineBindPoint graphics_pass_bind_point =
          VK_PIPELINE_BIND_POINT_GRAPHICS,
      uint32_t vertex_count = 3,
      uint32_t instance_count = 1,
      uint32_t first_vertex_index = 0,
      uint32_t first_instance_index = 0)
      : buffer(loc) {
    mk_cmd_buffer(
        sc_framebuffer, render_pass, swap_chain_extent,
        graphics_pipeline, vertex_buffer, index_buffer,
        indices, descriptor_set, pipeline_layout,
        render_offset_x, render_offset_y, clearColor,
        clearValueCount, subpass_contents,
        graphics_pass_bind_point, vertex_count,
        instance_count, first_vertex_index,
        first_instance_index);
  }
  vulkan_buffer(
      VkCommandBuffer loc,
      vulkan_buffer<VkFramebuffer> &sc_framebuffer,
      VkRenderPass &render_pass,
      VkExtent2D swap_chain_extent,
      VkPipeline graphics_pipeline, VkBuffer vertex_buffer,
      VkBuffer index_buffer, std::vector<uint32_t> indices,
      VkDescriptorSet descriptor_set,
      VkPipelineLayout pipeline_layout,
      VkCommandBufferBeginInfo beginInfo,
      VkRenderPassBeginInfo renderPassInfo,
      VkDrawInfo drawInfo,
      VkSubpassContents subpass_contents =
          VK_SUBPASS_CONTENTS_INLINE,
      VkPipelineBindPoint graphics_pass_bind_point =
          VK_PIPELINE_BIND_POINT_GRAPHICS)
      : buffer(loc) {
    //
    // mk_cmd_buffer(
    //    sc_framebuffer, render_pass, swap_chain_extent,
    //    graphics_pipeline, vertex_buffer, index_buffer,
    //    indices, beginInfo, renderPassInfo, drawInfo,
    //    subpass_contents, graphics_pass_bind_point);
  }
  void mk_cmd_buffer(
      vulkan_buffer<VkFramebuffer> &sc_framebuffer,
      VkRenderPass &render_pass,
      VkExtent2D swap_chain_extent,
      VkPipeline graphics_pipeline, VkBuffer vertex_buffer,
      VkBuffer index_buffer, std::vector<uint32_t> indices,
      VkDescriptorSet descriptor_set,
      VkPipelineLayout pipeline_layout,
      int32_t render_offset_x = 0,
      int32_t render_offset_y = 0,
      VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f},
      uint clearValueCount = 1,
      VkSubpassContents subpass_contents =
          VK_SUBPASS_CONTENTS_INLINE,
      VkPipelineBindPoint graphics_pass_bind_point =
          VK_PIPELINE_BIND_POINT_GRAPHICS,
      uint32_t vertex_count = 3,
      uint32_t instance_count = 1,
      uint32_t first_vertex_index = 0,
      uint32_t first_instance_index = 0) {

    // 1. create command buffer info
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType =
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    CHECK_VK(vkBeginCommandBuffer(buffer, &beginInfo),
             "failed to begin recording commands");

    // 2. create render pass info
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType =
        VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = render_pass;
    renderPassInfo.framebuffer = sc_framebuffer.buffer;
    renderPassInfo.renderArea.offset = {render_offset_x,
                                        render_offset_y};
    renderPassInfo.renderArea.extent = swap_chain_extent;

    std::array<VkClearValue, 2> cvalues{};
    cvalues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
    cvalues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount =
        static_cast<uint32_t>(cvalues.size());
    renderPassInfo.pClearValues = cvalues.data();

    // 3. give command to start rendering pass
    vkCmdBeginRenderPass(buffer, &renderPassInfo,
                         subpass_contents);

    // 4. bind pipeline to command buffer
    vkCmdBindPipeline(buffer, graphics_pass_bind_point,
                      graphics_pipeline);

    // 5. bind vertex buffer to command buffer
    VkBuffer vertex_buffers[] = {vertex_buffer};
    VkDeviceSize vertex_offsets[] = {0};
    vkCmdBindVertexBuffers(buffer, 0, 1, vertex_buffers,
                           vertex_offsets);
    // 6. bind index buffer to command buffer
    vkCmdBindIndexBuffer(buffer, index_buffer, 0,
                         VK_INDEX_TYPE_UINT32);

    // 7. bind descriptor set
    vkCmdBindDescriptorSets(
        buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline_layout, 0, 1, &descriptor_set, 0, nullptr);

    // 7. draw given command buffer with indices
    vkCmdDrawIndexed(buffer,
                     static_cast<uint32_t>(indices.size()),
                     instance_count, first_vertex_index,
                     first_instance_index, 0);

    vkCmdEndRenderPass(buffer);
    CHECK_VK(vkEndCommandBuffer(buffer),
             "failed to register command buffer");
  }
};
}
