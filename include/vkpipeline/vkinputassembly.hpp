// input assembly related
#include <external.hpp>
#include <vkutils/varia.hpp>

namespace vtuto {
typedef std::tuple<VkPrimitiveTopology, VkBool32> InputAssemblyArgs;
typedef std::tuple<std::optional<VkPipelineInputAssemblyStateCreateFlags>>
    InputAssemblyOpts;
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
template <>
struct VkStructSetter<VkPipelineInputAssemblyStateCreateInfo, // ObjType
                      InputAssemblyOpts,                      // Opts
                      InputAssemblyArgs                       // Args
                      > {
  static void set(VkPipelineInputAssemblyStateCreateInfo &createInfo,
                  InputAssemblyOpts &opts, InputAssemblyArgs &topologyRestart) {
    createInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    auto flagRefs = std::get<0>(opts);
    createInfo.topology = std::get<0>(topologyRestart);
    createInfo.primitiveRestartEnable = std::get<1>(topologyRestart);
    if (flagRefs.has_value()) {
      auto flags = flagRefs.value();
      createInfo.flags = flags;
    }
  }
};

} // namespace vtuto
