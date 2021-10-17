#pragma once
// functions related to making vkgraphs and vk graph components
#include <external.hpp>
#include <iterator>
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

template <std::size_t... PairIndices, typename Tuple>
constexpr std::array<BranchSignal, sizeof...(PairIndices)>
getBranchSignalHelper(std::index_sequence<PairIndices...>, Tuple tpl) {

  return std::array<BranchSignal, sizeof...(PairIndices)>{std::make_pair(
      std::get<2 * PairIndices>(tpl), std::get<2 * PairIndices + 1>(tpl))...};
}

template <std::size_t... PairIndices, typename Tuple>
constexpr std::array<unsigned int, sizeof...(PairIndices)>
getSignalHelper(std::index_sequence<PairIndices...>, Tuple tpl) {
  return std::array<unsigned int, sizeof...(PairIndices)>{
      std::get<2 * PairIndices>(tpl)...};
}

template <std::size_t... PairIndices, typename Tuple>
constexpr std::array<unsigned int, sizeof...(PairIndices)>
getNodeIdHelper(std::index_sequence<PairIndices...>, Tuple tpl) {
  return std::array<unsigned int, sizeof...(PairIndices)>{
      std::get<2 * PairIndices + 1>(tpl)...};
}

template <unsigned int... Bs>
constexpr std::array<BranchSignal, sizeof...(Bs) / 2> getBranchSignal() {
  constexpr auto tpl = std::make_tuple(Bs...);
  return getBranchSignalHelper(std::make_index_sequence<sizeof...(Bs) / 2>{},
                               tpl);
}
template <unsigned int... Bs>
constexpr std::array<unsigned int, sizeof...(Bs) / 2>
getSignalsFromBranchSignals() {
  constexpr auto tpl = std::make_tuple(Bs...);
  return getSignalHelper(std::make_index_sequence<sizeof...(Bs) / 2>{}, tpl);
}
template <unsigned int... Bs>
constexpr std::array<unsigned int, sizeof...(Bs) / 2>
getNodeIdsFromBranchSignals() {
  constexpr auto tpl = std::make_tuple(Bs...);
  return getNodeIdHelper(std::make_index_sequence<sizeof...(Bs) / 2>{}, tpl);
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
constexpr vk_node<VkApp>
mkNode(const std::function<vk_output(VkApp &)> &NodeFn) {
  vk_node<VkApp> n;
  n.is_singular = IsSingular;
  n.is_called = false;
  n.node_id = NodeId;
  n.compute = NodeFn;
  n.next_node = [](const vk_output &out) {
    const std::size_t BSize = sizeof...(Bs) / 2;
    constexpr std::array<BranchSignal, BSize> bs = getBranchSignal<Bs...>();
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
  constexpr std::array<unsigned int, sizeof...(Bs) / 2> arr =
      getNodeIdsFromBranchSignals<Bs...>();

  std::vector<unsigned int> ends(std::make_move_iterator(arr.begin()),
                                 std::make_move_iterator(arr.end()));
  auto mpair = std::make_pair(n.node_id, ends);
  auto npair = std::make_pair(n.node_id, n);
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (g.is_in(n.node_id)) {
    vr.status = FAIL_OP;
    vr.context = "node ";
    vr.context += std::to_string(n.node_id);
    vr.context += " already exists inside the graph";
    return vr;
  }
  g.adj_lst.insert(mpair);
  g.nodes.insert(npair);
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
