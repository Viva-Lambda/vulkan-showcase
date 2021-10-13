#pragma once
// functions related to making vkgraphs and vk graph components
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vknode.hpp>

using namespace vtuto;
namespace vtuto {

void addGraphOp(vk_graph &g, const vk_edge &e) { g.add_op(e); }

void mkVkGNode(unsigned int nid, std::function<void(vk_graph &)> &f,
               vk_node &n) {
  n = vk_node(nid, f);
}
void mkVkGEdge(unsigned int eid, vk_node &n1, vk_node &n2, unsigned int o,
               vk_edge &e) {
  if (n1.node_id == n2.node_id) {
    std::cerr << "nodes have same id " << std::endl;
    return;
  }
  e = vk_edge(eid, n1, n2, o);
}

void mkVkGEdge(unsigned int eid, vk_node &n1, vk_node &n2, vk_edge &e) {
  mkVkGEdge(eid, n1, n2, 0, e);
}
void mkVkGEdge(unsigned int start_id, std::function<void(vk_graph &)> &sf,
               unsigned int end_id, std::function<void(vk_graph &)> &ef,
               unsigned int edge_id, unsigned int order, vk_edge &edge) {
  vk_node start;
  mkVkGNode(start_id, sf, start);

  vk_node end;
  mkVkGNode(end_id, ef, end);

  // make the edge
  mkVkGEdge(edge_id, start, end, order, edge);
}

void mkVkGEdge(unsigned int start_id, std::function<void(vk_graph &)> &sf,
               unsigned int end_id, std::function<void(vk_graph &)> &ef,
               unsigned int edge_id, vk_edge &edge) {
  mkVkGEdge(start_id, sf, end_id, ef, edge_id,
            0, // order
            edge);
}
void mkAddVkGEdge(vk_graph &g, unsigned int start_id,
                  std::function<void(vk_graph &)> &sf, unsigned int end_id,
                  std::function<void(vk_graph &)> &ef, unsigned int edge_id,
                  unsigned int order) {
  vk_edge edge;
  mkVkGEdge(start_id, sf, end_id, ef, edge_id, order, edge);
  addGraphOp(g, edge);
}

void mkAddVkGEdge(vk_graph &g, vk_node &n1, vk_node &n2, unsigned int eid,
                  unsigned int o) {
  vk_edge e;
  mkVkGEdge(eid, n1, n2, o, e);
  addGraphOp(g, e);
}
void mkAddVkGEdge(vk_graph &g, vk_node &n1, vk_node &n2, unsigned int eid) {
  vk_edge e;
  mkVkGEdge(eid, n1, n2, e);
  addGraphOp(g, e);
}

} // namespace vtuto
