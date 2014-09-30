/*
 * test_counter_inverse_pagerank_node_factory.cpp
 *
 *  Created on: 2014.09.26.
 *      Author: kisstom
 */

#include "test_counter_inverse_pagerank_node_factory.h"
#include "../node_factory_helper.h"

Node* TestCounterInversePagerankNodeFactory::createNodeFromConfig(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  CounterInversePagerankNode* node = NULL;
  node = helper.initCounterInversePagerankNode(params);
  node->setOutpartitionIndices(outPartitionIndices);
  node->setPartitionBound(partitionBound);
  node->setPointerToCounters(pointerToCounters);
  node->setNumNeighbors(numNeighbors);

  return node;
}

void TestCounterInversePagerankNodeFactory::setPointerToCounters(EdgelistContainer* _pointerToCounters) {
  pointerToCounters = _pointerToCounters;
}

void TestCounterInversePagerankNodeFactory::setOutPartitionIndices(EdgelistContainer* _outPartitionIndices) {
  outPartitionIndices = _outPartitionIndices;
}

void TestCounterInversePagerankNodeFactory::setPartitionBound(vector<long>* _partitionBound) {
  partitionBound = _partitionBound;
}

void TestCounterInversePagerankNodeFactory::setNumNeighbors(vector<int>* _numNeighbors) {
  numNeighbors = _numNeighbors;
}

void TestCounterInversePagerankNodeFactory::setNumSlaves(int) {

}
