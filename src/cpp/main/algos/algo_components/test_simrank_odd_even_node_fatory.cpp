/*
 * test_simrank_odd_even_node_fatory.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "test_simrank_odd_even_node_factory.h"

TestSimrankOddEvenNodeFactory::TestSimrankOddEvenNodeFactory() {
  logger_ = &log4cpp::Category::getInstance(std::string("TestNodeFactory"));
  container_ = NULL;
}

Node* TestSimrankOddEvenNodeFactory::createNodeFromConfig(unordered_map<string, string>* params) {
  string nodeType = (*params)["NODE_TYPE"];
  Node* node = NULL;

  if (nodeType.compare("SIMRANK_ODD_EVEN") == 0) {
    node = createSimrankOddEvenNode(params);
  } else if (nodeType.compare("PSIMRANK") == 0) {
    node = createPSimrankNode(params);
  } else if (nodeType.compare("SIMPLE_MOCK") == 0) {
    node = createSimpleMockNode(params);
  } else {
    logger_->error("ERROR. Unknown type of algo %s.\n", nodeType.c_str());
  }

	return node;
}

Node* TestSimrankOddEvenNodeFactory::createSimrankOddEvenNode(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  SimrankOddEvenNode* node = NULL;
  node = helper.initSimrankOddEvenNode(params);
  node->setMatrix(container_);
  node->setFingerprints(fprints_);
  return node;
}

Node* TestSimrankOddEvenNodeFactory::createPSimrankNode(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  PSimrankNode* node = NULL;
  node = helper.initPSimrankNode(params);
  node->setMatrix(container_);
  node->setFingerprints(fprints_);
  return node;
}

Node* TestSimrankOddEvenNodeFactory::createSimpleMockNode(unordered_map<string, string>* params) {
  SimpleMockNode* node = new SimpleMockNode;
  return node;
}

void TestSimrankOddEvenNodeFactory::setEdgelistContainer(EdgelistContainer* container) {
	container_ = container;
}

void TestSimrankOddEvenNodeFactory::setFingerprints(vector<list<long*> > fprints) {
	fprints_ = fprints;
}


