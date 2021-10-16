#pragma once
// functions related to making vkgraphs and vk graph components
#include <external.hpp>
#include <utility>
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vknode.hpp>
#include <vkresult/debug.hpp>

using namespace vtuto;
namespace vtuto {

// typedef std::pair<unsigned int, unsigned int> BranchSignal;
/**
  adapted from SO: https://stackoverflow.com/a/41837912/7330813

  class Tpl is something like (1,8,3,6,1)
  the index sequence is (0,1,2,3,4)
 */
typedef std::pair<unsigned int, unsigned int> BranchSignal;
// first: signal, second: node id
template <unsigned int... Bs, class Tpl>
std::array<BranchSignal, sizeof...(Bs) / 2>
getBranchSignals(std::integer_sequence<unsigned int, Bs...>, Tpl ts) {
  return std::array<BranchSignal, sizeof...(Bs) / 2>{
      std::make_pair(std::get<2 * Bs>(ts), std::get<2 * Bs + 1>(ts))...};
}

template <unsigned int... Bs, class Tpl>
std::array<unsigned int, sizeof...(Bs) / 2>
getSignalsFromBranchSignal(std::index_sequence<Bs...>, Tpl tpl) {
  return std::array<unsigned int, sizeof...(Bs) / 2>{std::get<2 * Bs>(tpl)...};
}
template <unsigned int... Bs, class Tpl>
std::array<unsigned int, sizeof...(Bs) / 2>
getNodeIdsFromBranchSignals(std::index_sequence<Bs...>, Tpl tpl) {
  return std::array<unsigned int, sizeof...(Bs) / 2>{
      std::get<2 * Bs + 1>(tpl)...};
}
/**
  The idea is we pass the application type,
  the node id, whether it is a singular,
  and lastly its signals and slots as template parameters.

  The last one includes lightweight template magic.
  We specify the pairs of signals, node_ids in successive fashion like,
  1,2,6,3,7,2 and then we parse it as
  std::array<std::pair<signal, node_id>, 3>
 */
template <typename VkApp, unsigned int NodeId, bool IsSingular,
          unsigned int... Bs>
vk_node<VkApp> mkNode(const std::function<vk_output(VkApp &)> &NodeFn) {
  vk_node<VkApp> n;
  n.is_singular = IsSingular;
  n.is_called = false;
  n.node_id = NodeId;
  n.compute = NodeFn;
  n.next_node = [](const vk_output &out) {
    const std::size_t BSize = sizeof...(Bs) / 2;
    std::array<BranchSignal, BSize> bs =
        getBranchSignals(std::make_integer_sequence<unsigned int, BSize>{},
                         std::make_tuple(Bs...));
    for (const auto &b : bs) {
      if (b.first == out.signal) {
        return b.second;
      }
    }
    return static_cast<unsigned int>(0);
  };
  return n;
}

//
template <typename VkApp, unsigned int... Bs>
Result_Vk addNode(vk_graph<VkApp> &g, vk_node<VkApp> &n) {
  std::array<unsigned int, sizeof...(Bs) / 2> arr = getNodeIdsFromBranchSignals(
      std::make_index_sequence<sizeof...(Bs) / 2>{}, std::make_tuple(Bs...));

  std::vector<unsigned int> ends(arr.begin(), arr.end());
  auto mpair = std::make_pair(n.node_id, ends);
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (g.is_in(n.node_id)) {
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
          unsigned int... Bs>
Result_Vk mkAddNode(vk_graph<VkApp> &g,
                    const std::function<vk_output(VkApp &)> &NodeFn) {
  auto n = mkNode<VkApp, NodeId, IsSingular, Bs...>(NodeFn);
  return addNode<VkApp, Bs...>(g, n);
}
} // namespace vtuto
