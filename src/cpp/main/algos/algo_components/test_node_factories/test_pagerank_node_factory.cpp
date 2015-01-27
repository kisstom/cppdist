#include "test_pagerank_node_factory.h"
#include "../node_factory_helper.h"

TestPagerankNodeFactory::TestPagerankNodeFactory() {
  logger = &log4cpp::Category::getInstance(std::string("TestPagerankNodeFactory"));
  container = NULL;
  partConfHandler = NULL;
}

Node* TestPagerankNodeFactory::createNodeFromConfig(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  PagerankNode* node = helper.initPagerankNode(params);
  node->setEdgeListContainer(container);
  node->setPartitionConfigHandler(partConfHandler);

  int slaveIndex;
  sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slaveIndex);
  node->setPartitionIndex(slaveIndex);
  return node;
}

void TestPagerankNodeFactory::setEdgelistContainer(EdgelistContainer* _container) {
  container = _container;
}

void TestPagerankNodeFactory::setPartConfHandler(GraphPartitionConfigHandler* _partConfHandler) {
  partConfHandler = _partConfHandler;
}
