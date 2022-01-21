// vertex input state related
#include <external.hpp>

namespace vtuto {
/**
typedef struct VkPipelineVertexInputStateCreateInfo {

VkStructureType sType;

const void* pNext;

VkPipelineVertexInputStateCreateFlags flags;

uint32_t vertexBindingDescriptionCount;
const VkVertexInputBindingDescription* pVertexBindingDescriptions;

uint32_t vertexAttributeDescriptionCount;
const VkVertexInputAttributeDescription* pVertexAttributeDescriptions;

} VkPipelineVertexInputStateCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this structure.
- flags is reserved for future use.
- vertexBindingDescriptionCount is the number of vertex binding descriptions
provided in pVertexBindingDescriptions.
- pVertexBindingDescriptions is a pointer to an array of
VkVertexInputBindingDescription structures.
- vertexAttributeDescriptionCount is the number of vertex attribute
descriptions provided in pVertexAttributeDescriptions.
- pVertexAttributeDescriptions is a pointer to an array of
VkVertexInputAttributeDescription structures.

 */
struct PipelineVertexInputStateCreateInfoVk {

  VkPipelineVertexInputStateCreateInfo createInfo;

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

  PipelineVertexInputStateCreateInfoVk(
      std::optional<std::array<VkVertexInputBindingDescription>> bindingRefs,
      std::optional<std::array<VkVertexInputAttributeDescription>> attrRefs,
      std::optional<VkPipelineVertexInputStateCreateFlags> flagRefs) {
    createInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    if (bindingRefs.has_value()) {
      auto bindings = bindingRefs.value();
      createInfo.vertexBindingDescriptionCount =
          static_cast<std::uint32_t>(bindings.size());
      createInfo.pVertexBindingDescriptions = bindings.data();
    } else {
      createInfo.vertexBindingDescriptionCount = static_cast<std::uint32_t>(0);
    }
    if (attrRefs.has_value()) {
      auto attrs = attrRefs.value();
      createInfo.vertexAttributeDescriptionCount =
          static_cast<std::uint32_t>(attrs.size());
      createInfo.pVertexAttributeDescriptions = attrs.data();
    } else {
      createInfo.vertexAttributeDescriptionCount = 0;
    }
    if (flagRefs.has_value()) {
      createInfo.flags = flagRefs.value();
    }
  }
};
} // namespace vtuto
