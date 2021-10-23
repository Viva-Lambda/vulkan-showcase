#pragma once
// graph like architecture
#include <external.hpp>
#include <vkgraph/vkout.hpp>
#include <vkutils/litutils.hpp>
#include <vkutils/temputils.hpp>

namespace vtuto {

typedef unsigned int NodeIdVk;

template <class VkApp, class NextNodeT, NodeIdVk NodeId, bool IsSingular>
struct vk_cnode {
  const NodeIdVk node_id = NodeId;

  const_str label;

  std::function<vk_output(VkApp &)> compute;
  std::function<NextNodeT(const vk_output &)> next_node;
  const bool is_singular = IsSingular;
  bool is_called = false;

  constexpr vk_cnode(const const_str &nlabel) : label(nlabel) {}

  /**contains the result status of compute and next node to run*/
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

  /**contains the result status of compute and next node to run*/
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
