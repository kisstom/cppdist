#ifndef TEST_PAGERANK_NODE_FACTORY_H_
#define TEST_PAGERANK_NODE_FACTORY_H_

#include "../factories/old_partition_node_factory.h"
#include "../../pagerank/pagerank_node.h"

class TestPagerankNodeFactory: public OldPartitionNodeFactory {
public:
  TestPagerankNodeFactory();
  Node* createNodeFromConfig(unordered_map<string, string>* params);
  void setEdgelistContainer(EdgelistContainer*);
  void setPartConfHandler(GraphPartitionConfigHandler*);
private:
  EdgelistContainer* container;
  GraphPartitionConfigHandler* partConfHandler;
  log4cpp::Category* logger;
};

#endif  // TEST_PAGERANK_NODE_FACTORY_H_
