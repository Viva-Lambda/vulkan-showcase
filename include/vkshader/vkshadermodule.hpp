// shader module
#pragma once

#include <external.hpp>
#include <vkutils/ioutils.hpp>

namespace vtuto {
//
/**
// Provided by VK_VERSION_1_0
typedef struct VkShaderModuleCreateInfo {

VkStructureType sType;
const void* pNext;
VkShaderModuleCreateFlags flags;
size_t codeSize;
const uint32_t* pCode;
} VkShaderModuleCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this structure.
- flags is reserved for future use.
- codeSize is the size, in bytes, of the code pointed to by pCode.
- pCode is a pointer to code that is used to create the shader module. The
type and format of the code is determined from the content of the memory
addressed by pCode.
 */
struct ShaderModuleCreateInfoVk {
  VkShaderModuleCreateInfo createInfo;
  //

  template <typename T> void set(std::optional<T> &pNext) {
    if (pNext.has_value()) {
      auto val = pNext.value();
      if (std::is_pointer(T)) {
        createInfo.pNext = static_cast<void *>(val);
      } else {
        createInfo.pNext = static_cast<void *>(&val);
      }
    }
  }
  ShaderModuleCreateInfoVk(const std::array<char> &shaderCode) {
    //
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());
  }
  ShaderModuleCreateInfoVk(const std::string &shaderPath) {
    auto shaderCode = readFile(shaderPath);
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());
  }
};
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
- pNext is NULL or a pointer to a structure extending this structure.
- flags is a bitmask of VkPipelineShaderStageCreateFlagBits specifying how the
pipeline shader stage will be generated.
- stage is a VkShaderStageFlagBits value specifying a single pipeline stage.
- module is a VkShaderModule object containing the shader for this stage.
- pName is a pointer to a null-terminated UTF-8 string specifying the entry
point name of the shader for this stage.
- pSpecializationInfo is a pointer to a VkSpecializationInfo structure, as
described in Specialization Constants, or NULL.
 */
struct PipelineShaderStageCreateInfoVk {
  //
  VkPipelineShaderStageCreateInfo createInfo;
};

}; // namespace vtuto
