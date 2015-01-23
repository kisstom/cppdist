#include "abstract_node_factory.h"
#include "als_node_factory.h"
#include "als_multi_node_factory.h"
#include "als_multi_2step_factory.h"
#include "../node_factory.h"

AbstractNodeFactory::AbstractNodeFactory() {
  logger_ = &log4cpp::Category::getInstance(std::string("AbstractNodeFactory"));
}

INodeFactory* AbstractNodeFactory::provideNodeFactory(unordered_map<string, string>* params) {
  INodeFactory* nodeFactory = NULL;
  logger_->info("Using node type %s", (*params)["NODE_TYPE"].c_str());

  if ((*params)["NODE_TYPE"].compare("ALS") == 0) {
    AlsNodeFactory* concreteFactory = new AlsNodeFactory;
    AlsPartitionConfigHandler* handler = new AlsPartitionConfigHandler;
    handler->readItemConfig((*params)["REMOTE_DIR"] + "/item_part.cfg", atoi((*params)["NUM_SLAVES"].c_str()));
    handler->readUserConfig((*params)["REMOTE_DIR"] + "/user_part.cfg", atoi((*params)["NUM_SLAVES"].c_str()));
    concreteFactory->setPartitionConfigHandler(handler);
    nodeFactory = concreteFactory;
  } else if ((*params)["NODE_TYPE"].compare("ALS_MULTI") == 0) {
    AlsMultiNodeFactory* concreteFactory = new AlsMultiNodeFactory;
    AlsPartitionConfigHandler* handler = new AlsPartitionConfigHandler;
    handler->readItemConfig((*params)["REMOTE_DIR"] + "/item_part.cfg", atoi((*params)["NUM_SLAVES"].c_str()));
    handler->readUserConfig((*params)["REMOTE_DIR"] + "/user_part.cfg", atoi((*params)["NUM_SLAVES"].c_str()));
    concreteFactory->setPartitionConfigHandler(handler);
    nodeFactory = concreteFactory;
  } else if ((*params)["NODE_TYPE"].compare("ALS_MULTI_2STEP") == 0) {
    AlsMulti2StepFactory* concreteFactory = new AlsMulti2StepFactory;
    AlsPartitionConfigHandler* handler = new AlsPartitionConfigHandler;
    handler->readItemConfig((*params)["REMOTE_DIR"] + "/item_part.cfg", atoi((*params)["NUM_SLAVES"].c_str()));
    handler->readUserConfig((*params)["REMOTE_DIR"] + "/user_part.cfg", atoi((*params)["NUM_SLAVES"].c_str()));
    concreteFactory->setPartitionConfigHandler(handler);
    nodeFactory = concreteFactory;
  } else {
    NodeFactory* concreteFactory = new NodeFactory;
    GraphPartitionConfigHandler* handler = new GraphPartitionConfigHandler;
    string cfg = (*params)["REMOTE_DIR"] + "/" + (*params)["SLAVERY_CFG"];
    handler->readSlaveConfig(cfg, atoi((*params)["NUM_SLAVES"].c_str()));
    concreteFactory->setPartitionConfigHandler(handler);
    nodeFactory = concreteFactory;
  }

  return nodeFactory;
}
