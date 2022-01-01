#pragma once
// graph like architecture
#include <external.hpp>
#include <vkgraph/vknode.hpp>
#include <vkutils/litutils.hpp>

namespace vtuto {

template <class VkApp> struct vk_graph2 {
  std::map<NodeLabelVk, vk_tnode<VkApp>, const_comp<char>> nodes;

  bool is_in(const NodeLabelVk &label) const {
    if (nodes.count(label) == 1) {
      return true;
    } else {
      return false;
    }
  }

  std::string mkContextInfo(NodeLabelVk label, const std::string &msg) const {
    std::string nmsg = msg;
    nmsg += " node label ";
    nmsg += std::string(label.obj());
    return nmsg;
  }
  std::string mkContextInfo(NodeIdVk node_id, NodeLabelVk label,
                            const std::string &msg) const {
    std::string nmsg = msg;
    nmsg += " node id ";
    nmsg += std::to_string(node_id);
    nmsg += " node label ";
    nmsg += std::string(label.obj());
    return nmsg;
  }
  std::string mkContextInfo(NodeIdVk node_id, NodeLabelVk label,
                            NodeLabelVk nlabel, const std::string &msg) const {
    std::string nmsg = msg;
    nmsg += "\n current node id ";
    nmsg += std::to_string(node_id);
    nmsg += "\n current node label ";
    nmsg += std::string(label.obj());
    nmsg += "\n next node label ";
    nmsg += std::string(nlabel.obj());
    return nmsg;
  }

  std::string mkContextInfo(NodeLabelVk label, NodeLabelVk nlabel,
                            const std::string &msg) const {
    std::string nmsg = msg;
    nmsg += "\n current node label ";
    nmsg += std::string(label.obj());
    nmsg += "\n next node label ";
    nmsg += std::string(nlabel.obj());
    return nmsg;
  }

  Result_Vk bounds_check(NodeLabelVk slabel, NodeLabelVk elabel) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;
    if (!is_in(slabel)) {
      vr.status = GRAPH_ERROR;
      std::string nmsg = "start node does not exist in graph";
      std::string ctxt = mkContextInfo(slabel, nmsg);
      vr.context = ctxt;
      return vr;
    }

    if (!is_in(elabel)) {
      vr.status = GRAPH_ERROR;
      std::string nmsg = "end node does not exist in graph";
      std::string ctxt = mkContextInfo(elabel, nmsg);
      vr.context = ctxt;
      return vr;
    }
    return vr;
  }
  Result_Vk run_from_to(VkApp &app, NodeLabelVk start_node,
                        NodeLabelVk end_node) {
    //
    auto vr = bounds_check(start_node, end_node);
    if (vr.status != SUCCESS_OP) {
      return vr;
    }
    //
    auto node = nodes.at(start_node);
    while (node.label != end_node) {
      node.run(app);
      auto node_out = node.node_out;
      // operation was not successful output debugging information
      if (node_out.result_info.status != SUCCESS_OP) {
        std::string nmsg = "node computation failed";
        std::string gcontext = mkContextInfo(node.id, node.label, nmsg);
        node_out.result_info.context += "\n" + gcontext;
        return node_out.result_info;
      }
      next_info info = node.next();
      if (info.first.status != SUCCESS_OP) {
        std::string nmsg = "getting next node failed";
        std::string gcontext = mkContextInfo(node.id, node.label, nmsg);
        info.first.context += "\n" + gcontext;
        return info.first;
      }
      auto next_label = info.second;
      if (!is_in(next_label)) {
        // required node not in graph bye!
        vr.status = GRAPH_ERROR;
        std::string nmsg = "node computation results in non existing node";
        std::string ctxt = mkContextInfo(node.id, node.label, next_label, nmsg);
        vr.context = ctxt;
        return vr;
      }
      // next label is in graph
      node = vk_tnode<VkApp>(nodes.at(next_label));
    }
    vr.status = SUCCESS_OP;
    return vr;
  }
};

template <class VkApp, class NextNodeT> struct vk_graph {
  //
  std::map<unsigned int, vk_node<VkApp, NextNodeT>> nodes;
  std::map<unsigned int, std::vector<NextNodeT>> adj_lst;

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
  std::string mkContextInfo(unsigned int node_id, NextNodeT next_node_info,
                            const std::string &msg) const {
    std::string nmsg = msg;
    nmsg += "\n current node id ";
    nmsg += std::to_string(node_id);
    nmsg += "\n next node ";
    nmsg += std::to_string(next_node_info);
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
