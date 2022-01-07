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



}; // namespace vtuto
