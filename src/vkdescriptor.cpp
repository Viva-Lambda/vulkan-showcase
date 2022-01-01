// main file
#include <hellotriangle.hpp>

using namespace vtuto;

namespace vtuto {
void HelloTriangle::createDescriptorPool() {
  //
  std::array<VkDescriptorPoolSize, 2> poolSizes{};
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount =
      static_cast<uint32_t>(swap_chain.simages.size());
  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount =
      static_cast<uint32_t>(swap_chain.simages.size());

  // create descriptor pool
  VkDescriptorPoolCreateInfo pinfo{};
  pinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pinfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  pinfo.pPoolSizes = poolSizes.data();
  pinfo.maxSets = static_cast<uint32_t>(swap_chain.simages.size());

  CHECK_VK2(vkCreateDescriptorPool(logical_dev.device(), &pinfo, nullptr,
                                   &descriptor_pool),
            "failed to create descriptor pool");
}
void HelloTriangle::createDescriptorSets() {
  std::vector<VkDescriptorSetLayout> layouts(swap_chain.simages.size(),
                                             descriptor_set_layout);
  //
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptor_pool;
  allocInfo.descriptorSetCount =
      static_cast<uint32_t>(swap_chain.simages.size());
  allocInfo.pSetLayouts = layouts.data();

  descriptor_sets.resize(swap_chain.simages.size());
  CHECK_VK2(vkAllocateDescriptorSets(logical_dev.device(), &allocInfo,
                                     descriptor_sets.data()),
            "failed to allocate descriptor sets");

  //
  for (std::size_t i = 0; i < swap_chain.simages.size(); i++) {
    VkDescriptorBufferInfo binfo{};
    binfo.buffer = uniform_buffers[i];
    binfo.offset = 0;
    binfo.range = sizeof(UniformBufferObject);
    //
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture_image_view;
    imageInfo.sampler = texture_sampler;
    //
    std::array<VkWriteDescriptorSet, 2> dwset{};

    dwset[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dwset[0].dstSet = descriptor_sets[i];
    dwset[0].dstBinding = 0;
    dwset[0].dstArrayElement = 0;
    dwset[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    dwset[0].descriptorCount = 1;
    dwset[0].pBufferInfo = &binfo;
    //
    dwset[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dwset[1].dstSet = descriptor_sets[i];
    dwset[1].dstBinding = 1;
    dwset[1].dstArrayElement = 0;
    dwset[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    dwset[1].descriptorCount = 1;
    dwset[1].pImageInfo = &imageInfo;

    //
    vkUpdateDescriptorSets(logical_dev.device(),
                           static_cast<uint32_t>(dwset.size()), dwset.data(), 0,
                           nullptr);
  }
}
/** Descriptor layout for binding*/
void HelloTriangle::createDescriptorSetLayout() {
  //
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.pImmutableSamplers = nullptr;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 1;
  samplerLayoutBinding.descriptorCount = 1;
  samplerLayoutBinding.descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = nullptr;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding,
                                                          samplerLayoutBinding};
  //
  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings = bindings.data();

  CHECK_VK2(vkCreateDescriptorSetLayout(logical_dev.device(), &layoutInfo,
                                        nullptr, &descriptor_set_layout),
            "descriptor set layout creation failed");
}

} // namespace vtuto
