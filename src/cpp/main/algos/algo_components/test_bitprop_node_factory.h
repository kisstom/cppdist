/*
 * test_bitprop_node_factory.h
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

#ifndef TEST_BITPROP_NODE_FACTORY_H_
#define TEST_BITPROP_NODE_FACTORY_H_

#include "inode_factory.h"
#include "node_factory_helper.h"
#include "../bitprop/bitprop_node.h"

class TestBitpropNodeFactory : public INodeFactory {
public:
  Node* createNodeFromConfig(unordered_map<string, string>* params);
  void setContainer(EdgelistContainer*);
  void setFailedEstimateNodes(std::vector<FailedEstimate>*);
  void setEstimatonHandler(IEstimationHandler*);
  void setRandomBits(unsigned char* _randomBits);

private:
  std::vector<FailedEstimate>* failedEstimatedNodes;
  IEstimationHandler* estimationHandler;
  EdgelistContainer* container;
  unsigned char* randomBits;
};


#endif /* TEST_BITPROP_NODE_FACTORY_H_ */
