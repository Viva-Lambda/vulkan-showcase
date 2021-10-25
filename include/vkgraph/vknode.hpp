#pragma once
// graph like architecture
#include <external.hpp>
#include <vkgraph/vkout.hpp>
#include <vkutils/litutils.hpp>
#include <vkutils/temputils.hpp>

namespace vtuto {

typedef unsigned int NodeIdVk;
typedef const_str NodeLabelVk;

enum class BranchType : std::uint8_t { COND = 1, UNCOND = 2 };

typedef std::pair<BranchType, const_str> branch;

typedef std::pair<Result_Vk, const_str> next_info;

template <class VkApp> struct vk_tnode {
  NodeIdVk id;
  const_str label;

  const branch *outgoing_neigbours;
  const std::size_t nb_neighbours;

  const bool is_singular;
  bool is_called = false;

  std::pair<const char *, std::function<vk_output(VkApp &)>> task;

  /**contains the result status of compute and next node to
   * run*/
  vk_output node_out;

  template <std::size_t NbN>
  constexpr vk_tnode(
      NodeIdVk nid, const_str nlabel, bool s, const branch (&neighbours)[NbN],
      const std::pair<const char *, std::function<vk_output(VkApp &)>> &f)
      : id(nid), label(nlabel), outgoing_neigbours(neighbours),
        nb_neighbours(NbN), is_singular(s), task(f) {}
  constexpr vk_tnode(const vk_tnode<VkApp> &node)
      : id(node.id), label(node.label),
        outgoing_neigbours(node.outgoing_neigbours),
        nb_neighbours(node.nb_neighbours), is_singular(node.is_singular),
        is_called(node.is_called), task(node.task), node_out(node.node_out) {}

  void run(VkApp &g) {
    if (is_singular && !is_called) {
      // should be called once since it is a singular
      node_out = task.second(g);
      is_called = true;
    } else {
      // if not a singular recall the function
      node_out = task.second(g);
    }
  }
  next_info next() const {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    SignalVk s = node_out.signal - 1;
    if (s < 0) {
      vr.status = FAIL_OP;
      vr.context = "node signal is 0 or less. Indicating a failure in the "
                   "assigned computation or badly assigned neighbour";
      return std::make_pair(vr, const_str(""));
    }
    return std::make_pair(vr, outgoing_neigbours[s].second);
  }
};

template <class VkApp, class NextNodeT, NodeIdVk NodeId, bool IsSingular>
struct vk_cnode {
  const NodeIdVk node_id = NodeId;

  const_str label;

  std::function<vk_output(VkApp &)> compute;
  std::function<NextNodeT(const vk_output &)> next_node;
  const bool is_singular = IsSingular;
  bool is_called = false;

  constexpr vk_cnode(const const_str &nlabel) : label(nlabel) {}

  /**contains the result status of compute and next node to
   * run*/
  vk_output node_out;

  void run(VkApp &g) {
    if (is_singular && !is_called) {
      // should be called once since it is a singular
      node_out = compute(g);
      is_called = true;
    } else {
      // if not a singular recall the function
      node_out = compute(g);
    }
  }
  NextNodeT next() const { return next_node(node_out); }
};

template <class VkApp, class NextNodeT> struct vk_node {
  // default constructor
  vk_node() {}

  NodeIdVk node_id;

  std::function<vk_output(VkApp &)> compute;
  std::function<NextNodeT(const vk_output &)> next_node;
  bool is_singular = true;
  bool is_called = false;

  /**contains the result status of compute and next node to
   * run*/
  vk_output node_out;

  // real constructor
  vk_node(NodeIdVk n, const std::function<vk_output(VkApp &)> &f,
          const std::function<NextNodeT(const vk_output &)> &nn, bool isS)
      : node_id(n), compute(f), next_node(nn), is_singular(isS) {}
  void run(VkApp &g) {
    if (is_singular) {
      // should be called once since it is a singular
      if (!is_called) {
        node_out = compute(g);
        is_called = true;
      } else {
      }
    } else {
      // if not a singular recall the function
      node_out = compute(g);
    }
  }
  NextNodeT next() const { return next_node(node_out); }
};

} // namespace vtuto
