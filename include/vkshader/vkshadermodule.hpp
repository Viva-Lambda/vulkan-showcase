// shader module
#pragma once

#include <external.hpp>
#include <vkutils/ioutils.hpp>
#include <vkutils/temputils.hpp>
#include <vkutils/varia.hpp>

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
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is reserved for future use.
- codeSize is the size, in bytes, of the code pointed to by
pCode.
- pCode is a pointer to code that is used to create the
shader module. The
type and format of the code is determined from the content
of the memory
addressed by pCode.
 */
struct ShaderModuleCreateInfoVk {
  VkShaderModuleCreateInfo createInfo;
  //

  ShaderModuleCreateInfoVk(const std::vector<char> &shaderCode) {
    //
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode =
        reinterpret_cast<const std::uint32_t *>(shaderCode.data());
  }
  ShaderModuleCreateInfoVk(const std::string &shaderPath) {
    auto shaderCode = readFile(shaderPath);
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode =
        reinterpret_cast<const std::uint32_t *>(shaderCode.data());
  }
};

typedef std::tuple<std::optional<array_vk<VkPipelineShaderStageCreateFlagBits>>,
                   std::optional<std::string>,
                   std::optional<VkSpecializationInfo>>
    ShaderStageOpts;

typedef std::tuple<VkShaderStageFlagBits> ShaderStageFlags;

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
pipeline shader stage will be generated.
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
template <>
struct VkStructSetter<VkPipelineShaderStageCreateInfo, // ObjType
                      ShaderStageFlags,                // Flags
                      ShaderStageOpts,                 // Optional
                      VkShaderModule> {
  static void set(VkPipelineShaderStageCreateInfo &createInfo,
                  ShaderStageFlags &stageFlag, ShaderStageOpts &opts,
                  VkShaderModule &shaderModule) {
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = std::get<0>(stageFlag);
    auto flagRefs = std::get<0>(opts);
    if (flagRefs.has_value()) {
      auto flags = flagRefs.value();
      VkPipelineShaderStageCreateFlags f = flags.obj()[0];
      unsigned int size = static_cast<unsigned int>(flags.length());
      for (unsigned int i = 1; i < size; i++) {
        f |= flags.obj()[i];
      }
      createInfo.flags = f;
    } else {
      createInfo.flags = static_cast<VkPipelineShaderStageCreateFlags>(NULL);
    }
    //
    createInfo.module = shaderModule;

    //
    auto pname = std::get<1>(opts);
    if (pname.has_value()) {
      std::string sval = pname.value();
      createInfo.pName = sval.c_str();
    } else {
      createInfo.pName = "main";
    }
    //
    auto specialInfoRef = std::get<2>(opts);
    if (specialInfoRef.has_value()) {
      auto specialInfo = specialInfoRef.value();
      createInfo.pSpecializationInfo = &specialInfo;
    }
  }
};

}; // namespace vtuto
