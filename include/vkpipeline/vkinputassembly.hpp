// input assembly related
#include <external.hpp>

namespace vtuto {

/**

typedef struct VkPipelineInputAssemblyStateCreateInfo {

VkStructureType sType;

const void* pNext;

VkPipelineInputAssemblyStateCreateFlags flags;

VkPrimitiveTopology topology;

VkBool32 primitiveRestartEnable;
} VkPipelineInputAssemblyStateCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this structure.
- flags is reserved for future use.
- topology is a VkPrimitiveTopology defining the primitive topology, as
described below.
- primitiveRestartEnable controls whether a special vertex index value is
treated as restarting the assembly of primitives. This enable only applies to
indexed draws (vkCmdDrawIndexed, vkCmdDrawMultiIndexedEXT, and
vkCmdDrawIndexedIndirect), and the special index value is either 0xFFFFFFFF
when the indexType parameter of vkCmdBindIndexBuffer is equal to
VK_INDEX_TYPE_UINT32, 0xFF when indexType is equal to VK_INDEX_TYPE_UINT8_EXT,
or 0xFFFF when indexType is equal to VK_INDEX_TYPE_UINT16. Primitive restart
is not allowed for “list” topologies.
 */
struct PipelineInputAssemblyStateCerateInfoVk {
  VkPipelineInputAssemblyStateCerateInfo createInfo;

  template <typename T> void set(std::optional<T> &pNext) {
    createInfo.topology = topologyRef;
    if (pNext.has_value()) {
      auto val = pNext.value();
      if (std::is_pointer(T)) {
        createInfo.pNext = static_cast<void *>(val);
      } else {
        createInfo.pNext = static_cast<void *>(&val);
      }
    }
  }
  template <VkPrimitiveTopology topology, VkBool32 restartEnabled>
  PipelineInputAssemblyStateCerateInfoVk(
      std::optional<VkPipelineInputAssemblyStateCreateFlags> &flagRefs) {
    createInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    createInfo.topology = topology;
    createInfo.primitiveRestartEnable = restartEnabled;
    if (flagRefs.has_value()) {
      auto flags = flagRefs.value();
      createInfo.flags = flags;
    }
  }
};

} // namespace vtuto
