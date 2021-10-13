#pragma once
// graph like architecture
#include <external.hpp>

namespace vtuto {

struct vk_graph;

struct vk_node {
  // default constructor
  vk_node() {}

  unsigned int node_id;
  std::function<void(vk_graph &)> compute;
  bool is_singular = true;
  bool is_called = false;

  // real constructor
  vk_node(unsigned int n, std::function<void(vk_graph &)> &f)
      : node_id(n), compute(f) {}
  void run(vk_graph &g) {
    if (is_singular) {
      if (!is_called) {
        // if not a singular recall the function
        compute(g);
        is_called = true;
      }
    } else {
      compute(g);
    }
  }
};
} // namespace vtuto
