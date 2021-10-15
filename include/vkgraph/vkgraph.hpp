#pragma once
// graph like architecture
#include <external.hpp>
#include <vertex.hpp>
#include <vkgraph/vknode.hpp>

namespace vtuto {

template <class VkApp> struct vk_graph {
  //
  std::unordered_map<unsigned int, vk_node<VkApp>> nodes;
  std::unordered_map<unsigned int, std::vector<unsigned int>> adj_lst;

  bool is_in(unsigned int node_id) {
    if (nodes.count(node_id) == 1) {
      return true;
    } else {
      return false;
    }
  }
  Result_Vk run_from_to(unsigned int start_node, unsigned int end_node) {
    auto node = nodes[start_node];
    auto adj = adj_lst[start_node];
    while (node.node_id != end_node) {
      node.run();
      auto node_out = node.node_out;
      if (node_out.result_info.status != SUCCESS_OP) {
        return node_out.result_info;
      }
      // check if we are going to an unknown branch
      if (adj.find(node_out.next_node) != adj.end()) {
        node = nodes[node_out.next_node];
        adj = adj_lst[node_out.next_node];
      } else {
        Result_Vk vr;
        vr.status = GRAPH_ERROR;
        std::string next_n = std::to_string(node_out.next_node);
        std::string current_n = std::to_string(node.node_id);
        std::string nmsg = "node computation results in non adjacent branch";
        nmsg += " current node id: ";
        nmsg += current_n;
        nmsg += " output node id: ";
        nmsg += next_n;
        vr.context = nmsg;
        return vr;
      }
    }
  }
};
} // namespace vtuto
