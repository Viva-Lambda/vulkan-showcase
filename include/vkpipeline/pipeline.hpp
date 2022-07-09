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
    const std::optional<array_vk<
        VkPipelineShaderStageCreateFlagBits>> &bitsRef) {
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
}
