// vertex input state related
#include <external.hpp>
#include <vkutils/varia.hpp>

namespace vtuto {
typedef std::tuple<std::optional<array_vk<VkVertexInputBindingDescription>>,
                   std::optional<array_vk<VkVertexInputAttributeDescription>>,
                   std::optional<VkPipelineVertexInputStateCreateFlags>>
    VertexInputOpts;
// typedef std::tuple<> VertexInputFlags;
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
template <>
struct VkStructSetter<VkPipelineVertexInputStateCreateInfo, // ObjType
                      VertexInputOpts                       // Opts
                      > {
  static void set(VkPipelineVertexInputStateCreateInfo &createInfo,
                  VertexInputOpts &opts) {
    createInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingRefs = std::get<0>(opts);
    auto attrRefs = std::get<1>(opts);
    auto flagRefs = std::get<2>(opts);

    if (bindingRefs.has_value()) {
      auto bindings = bindingRefs.value();
      createInfo.vertexBindingDescriptionCount =
          static_cast<std::uint32_t>(bindings.length());
      createInfo.pVertexBindingDescriptions = bindings.obj();
    } else {
      createInfo.vertexBindingDescriptionCount = static_cast<std::uint32_t>(0);
    }
    if (attrRefs.has_value()) {
      auto attrs = attrRefs.value();
      createInfo.vertexAttributeDescriptionCount =
          static_cast<std::uint32_t>(attrs.length());
      createInfo.pVertexAttributeDescriptions = attrs.obj();
    } else {
      createInfo.vertexAttributeDescriptionCount =
          static_cast<std::uint32_t>(0);
    }
    if (flagRefs.has_value()) {
      createInfo.flags = flagRefs.value();
    }
  }
};
} // namespace vtuto
