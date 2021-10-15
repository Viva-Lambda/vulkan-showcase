#pragma once
// graph like architecture
#include <external.hpp>
#include <vkgraph/vkout.hpp>

namespace vtuto {

template <class VkApp> struct vk_node {
  // default constructor
  vk_node() {}

  unsigned int node_id;

  std::function<vk_output(VkApp &)> compute;
  bool is_singular = true;
  bool is_called = false;

  /**contains the result status of compute and next node to run*/
  vk_output node_out;

  // real constructor
  vk_node(unsigned int n, std::function<vk_output(VkApp &)> &f)
      : node_id(n), compute(f) {}
  vk_output run(VkApp &g) {
    if (is_singular) {
      // should be called once since it is a singular
      if (!is_called) {
        node_out = compute(g);
        is_called = true;
        return node_out;
      } else {
        return node_out;
      }
    } else {
      // if not a singular recall the function
      node_out = compute(g);
      return node_out;
    }
  }
};

} // namespace vtuto
