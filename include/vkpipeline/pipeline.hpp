// pipeline manager
#pragma once

#include <external.hpp>
#include <vkutils/varia.hpp>

namespace vtuto {
/**
typedef struct VkPipelineShaderStageCreateInfo {

VkStructureType sType;

const void* pNext;

VkPipelineShaderStageCreateFlags flags;

VkShaderStageFlagBits stage;

VkShaderModule module;

const char* pName;

const VkSpecializationInfo* pSpecializationInfo;
} VkPipelineShaderStageCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is a bitmask of VkPipelineShaderStageCreateFlagBits
specifying how the
pipeline shader
stage will be generated.
- stage is a VkShaderStageFlagBits value specifying a single
pipeline stage.
- module is a VkShaderModule object containing the shader
for this stage.
- pName is a pointer to a null-terminated UTF-8 string
specifying the entry
point name of the shader for this stage.
- pSpecializationInfo is a pointer to a VkSpecializationInfo
structure, as
described in Specialization Constants, or NULL.
 */
constexpr void
VkFlagSetter(VkPipelineShaderStageCreateInfo &createInfo,
             const VkShaderStageFlagBits &stageFlag,
             const VkShaderModule &smodule,
             const char *pname) {
  createInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  createInfo.stage = stageFlag;
  createInfo.module = smodule;
  createInfo.pName = pname;
}
void VkOptSetter(
    VkPipelineShaderStageCreateInfo &createInfo,
    const std::optional<
        array_vk<VkPipelineShaderStageCreateFlagBits>>
        &bitsRef = std::nullopt) {
  //
  if (bitsRef.has_value()) {
    auto fs = bitsRef.value();
    VkPipelineShaderStageCreateFlags f = fs.obj()[0];
    unsigned int size =
        static_cast<unsigned int>(fs.length());
    for (unsigned int i = 1; i < size; i++) {
      f |= fs.obj()[i];
    }
    createInfo.flags = f;
  }
}

/** VkPipelineVertexInputStateCreateInfo overloads */

/**
typedef struct VkPipelineVertexInputStateCreateInfo {

VkStructureType sType;

const void* pNext;

VkPipelineVertexInputStateCreateFlags flags;

uint32_t vertexBindingDescriptionCount;

const VkVertexInputBindingDescription*
pVertexBindingDescriptions;

uint32_t vertexAttributeDescriptionCount;

const VkVertexInputAttributeDescription*
pVertexAttributeDescriptions;
} VkPipelineVertexInputStateCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is reserved for future use.
- vertexBindingDescriptionCount is the number of vertex
binding descriptions provided in
pVertexBindingDescriptions.
- pVertexBindingDescriptions is a pointer to an array of
VkVertexInputBindingDescription
structures.
- vertexAttributeDescriptionCount is the number of vertex
attribute descriptions provided in
pVertexAttributeDescriptions.
- pVertexAttributeDescriptions is a pointer to an array of
VkVertexInputAttributeDescription structures.
 */

constexpr void VkFlagSetter(
    VkPipelineVertexInputStateCreateInfo &createInfo) {
  createInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
}
void VkOptSetter(
    VkPipelineVertexInputStateCreateInfo &createInfo,
    const std::optional<
        array_vk<VkVertexInputBindingDescription>>
        &binding_descriptions_ref = std::nullopt,
    const std::optional<
        array_vk<VkVertexInputAttributeDescription>>
        &attribute_descriptions_ref = std::nullopt) {
  if (binding_descriptions_ref.has_value()) {
    auto binding_descriptions =
        binding_descriptions_ref.value();
    createInfo.vertexBindingDescriptionCount =
        static_cast<std::uint32_t>(
            binding_descriptions.length());
    createInfo.pVertexBindingDescriptions =
        binding_descriptions.obj();
  } else {
    createInfo.vertexBindingDescriptionCount = 0;
  }
  if (attribute_descriptions_ref.has_value()) {
    auto attribute_descriptions =
        attribute_descriptions_ref.value();
    createInfo.vertexAttributeDescriptionCount =
        static_cast<std::uint32_t>(
            attribute_descriptions.length());
    createInfo.pVertexAttributeDescriptions =
        attribute_descriptions.obj();
  } else {
    createInfo.vertexAttributeDescriptionCount = 0;
  }
}

/** VkPipelineInputAssemblyStateCreateInfo overloads */

/**
typedef struct VkPipelineInputAssemblyStateCreateInfo {

VkStructureType sType;

const void* pNext;

VkPipelineInputAssemblyStateCreateFlags flags;

VkPrimitiveTopology topology;

VkBool32 primitiveRestartEnable;
} VkPipelineInputAssemblyStateCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is reserved for future use.
- topology is a VkPrimitiveTopology defining the primitive
topology, as described below.
- primitiveRestartEnable controls whether a special vertex
index value is
treated as restarting the assembly of primitives. This
enable only applies to
indexed draws (vkCmdDrawIndexed, vkCmdDrawMultiIndexedEXT,
and
vkCmdDrawIndexedIndirect), and the special index value is
either 0xFFFFFFFF
when the indexType parameter of vkCmdBindIndexBuffer is
equal to
VK_INDEX_TYPE_UINT32, 0xFF when indexType is equal to
VK_INDEX_TYPE_UINT8_EXT,
or 0xFFFF when indexType is equal to VK_INDEX_TYPE_UINT16.
Primitive restart
is not allowed for “list” topologies.
 */

constexpr void VkFlagSetter(
    VkPipelineInputAssemblyStateCreateInfo &createInfo,
    const VkPrimitiveTopology &topology,
    const VkBool32 &restart_with_primitive) {
  createInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  createInfo.topology = topology;
  createInfo.primitiveRestartEnable =
      restart_with_primitive;
}

/** VkPipelineMultisampleStateCreateInfo overloads */

/**
typedef struct VkPipelineMultisampleStateCreateInfo {

VkStructureType sType;

const void* pNext;

VkPipelineMultisampleStateCreateFlags flags;

VkSampleCountFlagBits rasterizationSamples;

VkBool32 sampleShadingEnable;

float minSampleShading;

const VkSampleMask* pSampleMask;
VkBool32 alphaToCoverageEnable;
VkBool32 alphaToOneEnable;

} VkPipelineMultisampleStateCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is reserved for future use.
- rasterizationSamples is a VkSampleCountFlagBits value
specifying the number of samples used
in rasterization.
- sampleShadingEnable can be used to enable Sample Shading.
- minSampleShading specifies a minimum fraction of sample
shading if sampleShadingEnable is set to
VK_TRUE.
- pSampleMask is a pointer to an array of VkSampleMask
values used in the sample mask test.
- alphaToCoverageEnable controls whether a temporary
coverage value is generated based on the alpha component of
the fragment’s
first color output as specified in the Multisample Coverage
section.
- alphaToOneEnable controls whether the alpha component of
the fragment’s
first color output is replaced with one as described in
Multisample Coverage.
 */
constexpr void VkFlagSetter(
    VkPipelineMultisampleStateCreateInfo &createInfo,
    const VkBool32 &sampleShadingEnable,
    const VkSampleCountFlagBits &rasterizationSamples) {

  createInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  createInfo.sampleShadingEnable = sampleShadingEnable;
  createInfo.rasterizationSamples = rasterizationSamples;
}

void VkOptSetter(
    VkPipelineMultisampleStateCreateInfo &createInfo,
    std::optional<float> minSampleShadingRef = std::nullopt,
    const std::optional<array_vk<VkSampleMask>>
        &pSampleMaskRef = std::nullopt,
    const std::optional<VkBool32>
        &alphaToCoverageEnableRef = std::nullopt,
    const std::optional<VkBool32> &alphaToOneEnableRef =
        std::nullopt) {
  if (minSampleShadingRef.has_value()) {
    auto minSampleShading = minSampleShadingRef.value();
    createInfo.minSampleShading = minSampleShading;
  }
  if (pSampleMaskRef.has_value()) {
    array_vk<VkSampleMask> pSampleMask_arr =
        pSampleMaskRef.value();
    const VkSampleMask *pSampleMask = pSampleMask_arr.obj();
    createInfo.pSampleMask = pSampleMask;
  }
  if (alphaToCoverageEnableRef.has_value()) {
    createInfo.alphaToCoverageEnable =
        alphaToCoverageEnableRef.value();
  }
  if (alphaToOneEnableRef.has_value()) {
    createInfo.alphaToOneEnable =
        alphaToOneEnableRef.value();
  }
}
// void VkOptSetter() {}
}
