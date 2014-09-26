/*
 * test_node_factory.cpp
 *
 *  Created on: 2014.09.26.
 *      Author: kisstom
 */

#include "test_counter_inverse_node_factory.h"
#include "../node_factory_helper.h"

Node* TestCounterInverseNodeFactory::createNodeFromConfig(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  CounterInverseNode* node = NULL;
  node = helper.initCounterInverseNode(params);
  node->setEdgeListContainer(container);
  node->setCounters(numSlaves);
  return node;
}

void TestCounterInverseNodeFactory::
setEdgelistContainer(EdgelistContainer* _container) {
  container = _container;
}

void TestCounterInverseNodeFactory::setNumSlaves(int _numSlaves) {
  numSlaves = _numSlaves;
}
