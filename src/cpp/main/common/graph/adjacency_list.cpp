#include "adjacency_list.h"
#include "adjacency_list_iterator_part.h"

template <>
void AdjacencyList<Entry>::flush(FILE* out) const {
  AdjacencyListIteratorPart<Entry> it = createIteratorPart();
  Entry e;

  for (long i = 0; i < getNumberOfNodes(); ++i) {
    it.resetRow(i);
    while (it.hasNext()) {
      e = it.next();
      fprintf(out, "%ld %ld %lf\n", minnode_ + i, e.id, e.value);
    }
  }
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
