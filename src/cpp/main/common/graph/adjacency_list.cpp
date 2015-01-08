#include "adjacency_list.h"
#include "entry.h"

template <>
const Entry& AdjacencyList<Entry>::getEdgeAtPos(long node, int index) const {
  long nodeIndex = node - minnode_;
  if (nodeIndex < 0) return Entry::getNullItem();

  return edge_list_->at(start_edges_->at(nodeIndex) + index);
}

template <>
const Entry& AdjacencyList<Entry>::getEdgeAtPosPart(long node, int index) const {
  if (node < 0) return Entry::getNullItem();

  return edge_list_->at(start_edges_->at(node) + index);
}

template <>
const long& AdjacencyList<long>::getEdgeAtPos(long node, int index) const {
  long nodeIndex = node - minnode_;
  //if (nodeIndex < 0) return -1;

  return edge_list_->at(start_edges_->at(nodeIndex) + index);
}

template <>
const long& AdjacencyList<long>::getEdgeAtPosPart(long node, int index) const {
  //if (node < 0) return -1;

  return edge_list_->at(start_edges_->at(node) + index);
}
