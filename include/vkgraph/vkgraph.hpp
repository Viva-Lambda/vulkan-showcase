#pragma once
// graph like architecture
#include <external.hpp>
#include <vertex.hpp>
#include <vkgraph/vknode.hpp>

namespace vtuto {

template <class VkApp> struct vk_graph {
  //
  std::map<unsigned int, vk_node<VkApp>> nodes;
  std::map<unsigned int, std::vector<unsigned int>> adj_lst;

  bool is_in(unsigned int node_id) const {
    if (nodes.count(node_id) == 1) {
      return true;
    } else {
      return false;
    }
  }
  std::string mkContextInfo(unsigned int node_id,
                            const std::string &msg) const {
    std::string nmsg = msg;
    nmsg += " node id ";
    nmsg += std::to_string(node_id);
    return nmsg;
  }
  std::string mkContextInfo(unsigned int node_id, unsigned int next_node_id,
                            const std::string &msg) const {
    std::string nmsg = msg;
    nmsg += "\n current node id ";
    nmsg += std::to_string(node_id);
    nmsg += "\n next node id ";
    nmsg += std::to_string(next_node_id);
    return nmsg;
  }
  Result_Vk run_from_to(VkApp &app, unsigned int start_node,
                        unsigned int end_node) {
    if (!is_in(start_node) || !is_in(end_node)) {
      Result_Vk vr;
      vr.status = GRAPH_ERROR;
      std::string nmsg = "start or end node does not exist in graph";
      std::string ctxt = mkContextInfo(start_node, end_node, nmsg);
      vr.context = ctxt;
      return vr;
    } else if (adj_lst.count(start_node) == 0 || adj_lst.count(end_node) == 0) {
      Result_Vk vr;
      vr.status = GRAPH_ERROR;
      std::string nmsg = "start or end node does not exist in adjacency list";
      std::string ctxt = mkContextInfo(start_node, end_node, nmsg);
      vr.context = ctxt;
      return vr;
    }
    auto node = nodes.at(start_node);
    auto adj = adj_lst.at(start_node);
    while (node.node_id != end_node) {
      node.run(app);
      auto node_out = node.node_out;
      // operation was not successful output debugging information
      if (node_out.result_info.status != SUCCESS_OP) {
        std::string nmsg = "node computation failed";
        std::string gcontext = mkContextInfo(node.node_id, nmsg);
        node_out.result_info.context += "\n" + gcontext;
        return node_out.result_info;
      }
      // check if we are going to an unknown branch
      unsigned int next_node = node.next();
      if (std::find(adj.begin(), adj.end(), next_node) != adj.end()) {
        node = nodes.at(next_node);
        adj = adj_lst.at(next_node);
      } else {
        Result_Vk vr;
        vr.status = GRAPH_ERROR;
        std::string nmsg = "node computation results in non adjacent branch";
        std::string ctxt = mkContextInfo(node.node_id, next_node, nmsg);
        vr.context = ctxt;
        return vr;
      }
    }
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    return vr;
  }
};
} // namespace vtuto
