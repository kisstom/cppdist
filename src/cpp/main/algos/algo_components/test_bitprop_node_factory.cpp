/*
 * test_bitprop_node_factory.cpp
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

#include "test_bitprop_node_factory.h"

Node* TestBitpropNodeFactory::createNodeFromConfig(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  BitpropNode* node = helper.initBitpropNode(params);

  node->setFailedEstimateNodes(failedEstimatedNodes);
  node->setEstimatonHandler(estimationHandler);
  node->setContainer(container);
  node->initContainers();

  return node;
}

void TestBitpropNodeFactory::setContainer(EdgelistContainer* _container) {
  container = _container;
}

void TestBitpropNodeFactory::setFailedEstimateNodes(std::vector<FailedEstimate>* _failedEstimatedNodes) {
  failedEstimatedNodes = _failedEstimatedNodes;
}

void TestBitpropNodeFactory::setEstimatonHandler(IEstimationHandler* _estimationHandler) {
  estimationHandler = _estimationHandler;
}
