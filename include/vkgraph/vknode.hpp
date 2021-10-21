#pragma once
// graph like architecture
#include <external.hpp>
#include <vkgraph/vkout.hpp>
#include <vkutils/litutils.hpp>

namespace vtuto {

template <class VkApp, class NextNodeT, unsigned int NodeId, bool IsSingular,
          bool IsCallable>
struct vk_cnode {
  const unsigned int node_id = NodeId;

  const char *label;
  std::size_t len;

  std::function<vk_output(VkApp &)> compute;
  std::function<NextNodeT(const vk_output &)> next_node;
  const bool is_singular = IsSingular;
  bool is_called = IsCallable;

  template <std::size_t N>
  constexpr vk_cnode(const char *(&a)[N]) : label(a), len(N - 1) {}

  /**contains the result status of compute and next node to run*/
  vk_output node_out;

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

template <class VkApp> struct vk_node {
  // default constructor
  vk_node() {}

  unsigned int node_id;

  std::function<vk_output(VkApp &)> compute;
  std::function<unsigned int(const vk_output &)> next_node;
  bool is_singular = true;
  bool is_called = false;

  /**contains the result status of compute and next node to run*/
  vk_output node_out;

  // real constructor
  vk_node(unsigned int n, const std::function<vk_output(VkApp &)> &f,
          const std::function<unsigned int(const vk_output &)> &nn)
      : node_id(n), compute(f), next_node(nn) {}
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
  unsigned int next() const { return next_node(node_out); }
};

} // namespace vtuto
