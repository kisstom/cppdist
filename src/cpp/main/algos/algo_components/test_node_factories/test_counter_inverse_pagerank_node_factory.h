/*
 * test_counter_inverse_pagerank_node_factory.h
 *
 *  Created on: 2014.09.26.
 *      Author: kisstom
 */

#ifndef TEST_COUNTER_INVERSE_PAGERANK_NODE_FACTORY_H_
#define TEST_COUNTER_INVERSE_PAGERANK_NODE_FACTORY_H_

#include "../inode_factory.h"
#include "../../counter_inverse_pagerank/counter_inverse_pagerank_node.h"

class TestCounterInversePagerankNodeFactory: public INodeFactory {
public:
  Node* createNodeFromConfig(unordered_map<string, string>* params);
  void setPointerToCounters(EdgelistContainer*);
  void setOutPartitionIndices(EdgelistContainer*);
  void setPartitionBound(vector<long>*);
  void setNumSlaves(int);
  void setNumNeighbors(vector<int>* numNeighbors);
private:
  int numSlaves;
  EdgelistContainer* pointerToCounters;
  EdgelistContainer* outPartitionIndices;
  vector<long>* partitionBound;
  vector<int>* numNeighbors;
  log4cpp::Category* logger;
};


#endif /* TEST_COUNTER_INVERSE_PAGERANK_NODE_FACTORY_H_ */
