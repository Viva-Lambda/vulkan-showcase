#pragma once
// functions related to making vkgraphs and vk graph components
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vknode.hpp>
#include <vkresult/debug.hpp>

using namespace vtuto;
namespace vtuto {
// node making functions
template <typename VkApp, unsigned int NodeId, bool IsSingular>
vk_node<VkApp> mkNode(const std::function<vk_output(VkApp &)> &NodeFn) {
  vk_node<VkApp> n;
  n.is_singular = IsSingular;
  n.is_called = false;
  n.node_id = NodeId;
  n.compute = NodeFn;
  return n;
}
//
template <typename VkApp, unsigned int... Ends>
Result_Vk addNode(vk_graph<VkApp> &g, vk_node<VkApp> &n) {
  std::vector<unsigned int> ends = {Ends...};
  auto mpair = std::make_pair(n.node_id, ends);
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (g.is_in(n)) {
    vr.status = FAIL_OP;
    vr.context = "node ";
    vr.context += std::to_string(n.node_id);
    vr.context += "already exists inside the graph";
    return vr;
  }
  g.adj_lst.insert(mpair);
  g.nodes.insert(n.node_id, n);
  return vr;
}
template <typename VkApp, unsigned int NodeId, bool IsSingular,
          unsigned int... Ends>
Result_Vk mkAddNode(vk_graph<VkApp> &g,
                    const std::function<vk_output(VkApp &)> &NodeFn) {
  auto n = mkNode<VkApp, NodeId, IsSingular>(NodeFn);
  return addNode<VkApp, Ends...>(g, n);
}
} // namespace vtuto
