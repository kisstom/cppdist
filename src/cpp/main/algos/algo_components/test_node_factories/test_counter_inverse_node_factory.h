/*
 * test_node_factory.h
 *
 *  Created on: 2014.09.26.
 *      Author: kisstom
 */

#ifndef TEST_COUNTER_INVERSE_NODE_FACTORY_H_
#define TEST_COUNTER_INVERSE_NODE_FACTORY_H_

#include "../inode_factory.h"
#include "../../counter_inverse/counter_inverse_node.h"

class TestCounterInverseNodeFactory: public INodeFactory {
public:
  Node* createNodeFromConfig(unordered_map<string, string>* params);
  void setEdgelistContainer(EdgelistContainer*);
  void setNumSlaves(int);
private:
  int numSlaves;
  EdgelistContainer* container;
  CounterInverseNode* createCounterInverseNode(unordered_map<string, string>*);
  log4cpp::Category* logger;
};


#endif /* TEST_NODE_FACTORY_H_ */
