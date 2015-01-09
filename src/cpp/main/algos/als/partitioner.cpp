#include "partitioner.h"

short Partitioner::getPartitionIndex(long node) {
  for (int i = partition_min_node_->size() - 1; i >= 0; --i) {
    if ((*partition_min_node_)[i] <= node) return i;
  }
  return 0;
}

void Partitioner::setPartMinNodes(vector<long>* _partMinNode) {
  partition_min_node_ = _partMinNode;
}
