#pragma once
#include <external.hpp>
#include <vkgraph/vkout.hpp>
#include <vkutils/litutils.hpp>
#include <vkutils/temputils.hpp>

namespace vtuto {

typedef unsigned int TaskIdVk;

template <class VkApp, TaskIdVk TaskId, bool IsSingular> struct vk_ctask {
  const TaskIdVk id = TaskId;
  const_str label;

  const bool is_singular = IsSingular;
  bool is_called = false;

  std::function<vk_output(VkApp &)> compute;
  const const_str *dependencies;

  template <std::size_t NbDeps>
  constexpr vk_ctask(const const_str &nlabel, const const_str (&deps)[NbDeps])
      : label(nlabel), dependencies(deps) {}

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
};

} // namespace vtuto
