#pragma once
// functions related to making vkgraphs and vk graph components
#include <external.hpp>
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vknode.hpp>
#include <vkresult/debug.hpp>
#include <vkutils/litutils.hpp>
#include <vkutils/temputils.hpp>

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

struct BranchSignal2 {
  const const_str *targets;
  const_uints signals;
  const std::size_t nb_targets;

  template <std::size_t NbTarget>
  constexpr BranchSignal2(const const_str (&a)[NbTarget],
                          const const_uints &ints)
      : targets(a), signals(ints), nb_targets(NbTarget) {}
};

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
constexpr vk_node<VkApp, NodeIdVk>
mkNode(const std::function<vk_output(VkApp &)> &NodeFn) {
  vk_node<VkApp, NodeIdVk> n;
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

template <> struct FnOut<const vk_output &> { using type = unsigned int; };

template <typename VkApp, typename NextNodeT, NodeIdVk NodeId, bool IsSingular,
          unsigned int... Bs>
constexpr vk_node<VkApp, NextNodeT>
mkNode2(const std::function<vk_output(VkApp &)> &NodeFn,
        const const_str &nlabel) {
  constexpr vk_cnode<VkApp, NextNodeT, NodeId, IsSingular> cnode(nlabel);
  cnode.compute = NodeFn;
  FnOut<const vk_output &>::type nfunc = [](const vk_output &out) {
    //
    const std::size_t BSize = sizeof...(Bs) / 2;
    constexpr std::array<BranchSignal, BSize> bs = getBranchSignal<Bs...>();
    for (const auto &b : bs) {
      if (b.first == out.signal) {
        return b.second;
      }
    }
    return static_cast<unsigned int>(0);
  };
  cnode.next_node = nfunc;
  vk_node<VkApp, NextNodeT> node(cnode.node_id, cnode.compute, cnode.next_node);
  node.is_singular = cnode.is_singular;
  return node;
}
template <typename VkApp, typename NextNodeT, NodeIdVk NodeId, bool IsSingular>
constexpr vk_node<VkApp, NextNodeT>
mkNode2(const std::function<vk_output(VkApp &)> &NodeFn,
        const std::function<NextNodeT(const vk_output &)> &nfunc,
        const const_str &nlabel) {
  vk_cnode<VkApp, NextNodeT, NodeId, IsSingular> cnode(nlabel);
  cnode.compute = NodeFn;
  cnode.next_node = nfunc;
  vk_node<VkApp, NextNodeT> node(cnode.node_id, cnode.compute, cnode.next_node,
                                 cnode.is_singular);
  return node;
}

template <typename VkApp, typename NextNodeT>
Result_Vk addNode2(vk_graph<VkApp, NextNodeT> &g,
                   const vk_node<VkApp, NextNodeT> &n,
                   const BranchSignal2 &bsignals) {
  //
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (g.is_in(n.node_id)) {
    vr.status = FAIL_OP;
    vr.context = "node ";
    vr.context += std::to_string(n.node_id);
    vr.context += " already exists inside the graph";
    return vr;
  }
  std::vector<const_str> arr(bsignals.targets,
                             bsignals.targets + bsignals.nb_targets);
  auto mpair = std::make_pair(n.node_id, arr);
  auto npair = std::make_pair(n.node_id, n);
  g.adj_lst.insert(mpair);
  g.nodes.insert(npair);
  return vr;
}

//
template <typename VkApp, unsigned int... Bs>
Result_Vk addNode(vk_graph<VkApp, NodeIdVk> &g, vk_node<VkApp, NodeIdVk> &n) {
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

template <typename VkApp, NodeIdVk NodeId, bool IsSingular, unsigned int... Bs>
Result_Vk mkAddNode(vk_graph<VkApp, NodeIdVk> &g,
                    const std::function<vk_output(VkApp &)> &NodeFn) {
  auto n = mkNode<VkApp, NodeId, IsSingular, Bs...>(NodeFn);
  return addNode<VkApp, Bs...>(g, n);
}

template <typename VkApp, NodeIdVk NodeId, class NextNodeT, bool IsSingular>
Result_Vk mkAddNode(vk_graph<VkApp, NextNodeT> &g,
                    const std::function<vk_output(VkApp &)> &NodeFn,
                    const std::function<NextNodeT(const vk_output &)> &nnf,
                    const const_str &nlabel, const BranchSignal2 &b) {
  auto n = mkNode2<VkApp, NextNodeT, NodeId, IsSingular>(NodeFn, nnf, nlabel);
  return addNode2<VkApp, NextNodeT>(g, n, b);
}

} // namespace vtuto
