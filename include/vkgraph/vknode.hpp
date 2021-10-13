#pragma once
// graph like architecture
#include <external.hpp>

namespace vtuto {

struct vk_node {
  // default constructor
  vk_node() {}

  unsigned int node_id;
  std::function<void(void)> compute;

  // real constructor
  vk_node(unsigned int n, std::function<void(void)> &f)
      : node_id(n), compute(f) {}
};
} // namespace vtuto
