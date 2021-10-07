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
void HelloTriangle::createVertexBuffer() {
  // 1. buffer related info
  auto device_size = vertices.size() * sizeof(vertices[0]);
  VkDeviceSize vk_device_size = device_size;
  auto mem_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  // 2. creating stage buffer
  VkBuffer staging_buffer;
  VkDeviceMemory staging_memory;
  auto stage_usage_flag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

  // create staging buffer
  createBuffer(device_size, stage_usage_flag, mem_flags, staging_buffer,
               staging_memory);

  void *data;
  vkMapMemory(logical_dev.device(), staging_memory, 0, device_size, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(device_size));
  vkUnmapMemory(logical_dev.device(), staging_memory);

  // 3. declare vertex buffer
  auto vertex_usage_flag =
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  auto vertex_mem_flag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

  createBuffer(device_size, vertex_usage_flag, vertex_mem_flag, vertex_buffer,
               vertex_buffer_memory);

  copyBuffer(staging_buffer, vertex_buffer, device_size);

  vkDestroyBuffer(logical_dev.device(), staging_buffer, nullptr);
  vkFreeMemory(logical_dev.device(), staging_memory, nullptr);
}
void HelloTriangle::createIndexBuffer() {
  // 1. buffer related info
  VkDeviceSize size = indices.size() * sizeof(indices[0]);

  VkBuffer staging_buffer;
  VkDeviceMemory staging_memory;
  auto stage_usage_flag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

  auto mem_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  // create staging buffer
  createBuffer(size, stage_usage_flag, mem_flags, staging_buffer,
               staging_memory);
  void *data;
  vkMapMemory(logical_dev.device(), staging_memory, 0, size, 0, &data);
  memcpy(data, indices.data(), static_cast<size_t>(size));
  vkUnmapMemory(logical_dev.device(), staging_memory);

  // 3. declare index buffer
  auto index_usage_flag =
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  auto index_mem_flag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

  createBuffer(size, index_usage_flag, index_mem_flag, index_buffer,
               index_buffer_memory);

  copyBuffer(staging_buffer, index_buffer, size);
  vkDestroyBuffer(logical_dev.device(), staging_buffer, nullptr);
  vkFreeMemory(logical_dev.device(), staging_memory, nullptr);
}
void HelloTriangle::copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) {
  //
  VkCommandBuffer cbuffer = beginSignalCommand();

  // start recording commands
  VkBufferCopy copyRegion{};
  copyRegion.size = size;
  vkCmdCopyBuffer(cbuffer, src, dst, 1, &copyRegion);

  // end signal
  endSignalCommand(cbuffer);
}
void HelloTriangle::createUniformBuffer() {
  VkDeviceSize b_size = sizeof(UniformBufferObject);

  uniform_buffers.resize(swap_chain.simages.size());
  uniform_buffer_memories.resize(swap_chain.simages.size());
  for (std::size_t i = 0; i < swap_chain.simages.size(); i++) {
    //
    auto usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    auto mem_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    createBuffer(b_size, usage, mem_flags, uniform_buffers[i],
                 uniform_buffer_memories[i]);
  }
}
/**
  abstract buffer creation mechanism
 */
void HelloTriangle::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                                 VkMemoryPropertyFlags mem_flags,
                                 VkBuffer &buffer,
                                 VkDeviceMemory &buffer_memory) {
  // 1. create buffer info
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  CHECK_VK2(vkCreateBuffer(logical_dev.device(), &bufferInfo, nullptr, &buffer),
            "buffer creation failed");

  // 2. query memory requirements
  VkMemoryRequirements memReq;
  vkGetBufferMemoryRequirements(logical_dev.device(), buffer, &memReq);

  // 3. allocate required memory
  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memReq.size;
  allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, mem_flags);

  CHECK_VK2(vkAllocateMemory(logical_dev.device(), &allocInfo, nullptr,
                             &buffer_memory),
            "failed to allocate memory from logical device");

  // 4. map host to device memory
  vkBindBufferMemory(logical_dev.device(), buffer, buffer_memory, 0);
}

void HelloTriangle::createCommandBuffers() {
  cmd_buffers.resize(swapchain_framebuffers.size());

  VkCommandBufferAllocateInfo comAllocInfo{};
  comAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  comAllocInfo.commandPool = command_pool.pool;
  comAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  comAllocInfo.commandBufferCount = static_cast<uint32_t>(cmd_buffers.size());
  //
  CHECK_VK2(vkAllocateCommandBuffers(logical_dev.device(), &comAllocInfo,
                                     cmd_buffers.data()),
            "failed allocate for registering command buffers");

  //
  for (std::size_t i = 0; i < cmd_buffers.size(); i++) {
    //
    auto buffer = vulkan_buffer<VkCommandBuffer>(
        cmd_buffers.get(i), swapchain_framebuffers[i], render_pass,
        swap_chain.sextent, graphics_pipeline, vertex_buffer, index_buffer,
        indices, descriptor_sets[i], pipeline_layout);
  }
}
} // namespace vtuto
