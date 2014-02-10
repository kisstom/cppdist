/*
 * test_simrank_odd_even_node_fatory.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "test_simrank_odd_even_node_factory.h"

Node* TestSimrankOddEvenNodeFactory::createNodeFromConfig(unordered_map<string, string>* params) {
	NodeFactoryHelper helper;
	SimrankOddEvenNode* node = helper.initSimrankOddEvenNode(params);
	node->setMatrix(container_);
  node->setFingerprints(fprints_);
	return node;
}

void TestSimrankOddEvenNodeFactory::setEdgelistContainer(EdgelistContainer* container) {
	container_ = container;
}

void TestSimrankOddEvenNodeFactory::setFingerprints(vector<list<long*> > fprints) {
	fprints_ = fprints;
}


