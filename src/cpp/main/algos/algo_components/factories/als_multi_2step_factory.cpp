#include "als_multi_2step_factory.h"
#include "../../als_multi_2step/als_multi_2step.h"
#include "../../../common/graph/builder/adjacency_list_builder.h"
#include "../../../common/graph/adjacency_list.h"

AlsMulti2StepFactory::AlsMulti2StepFactory() {
  partConfigHandler = NULL;
  logger_ = &log4cpp::Category::getInstance(std::string("AlsMulti2StepFactory"));
}

Node* AlsMulti2StepFactory::createNodeFromConfig(
    unordered_map<string, string>* params) {
  AlsMulti2Step* node = new AlsMulti2Step(params);

  string userPartFile = (*params)["REMOTE_DIR"] + "/user_part_"
      + (*params)["SLAVE_INDEX"];
  string itemPartFile = (*params)["REMOTE_DIR"] + "/item_part_"
      + (*params)["SLAVE_INDEX"];

  // Sets partition.
  AdjacencyListBuilder builder;
  AdjacencyList<Entry>* userPartition = new AdjacencyList<Entry>();
  userPartition->setMinnode(partConfigHandler->
      getUserMinNode(util.stringToInt((*params)["SLAVE_INDEX"])));
  userPartition->initContainers();
  builder.buildFromFile(userPartFile, userPartition);

  AdjacencyList<Entry>* itemPartition = new AdjacencyList<Entry>();
  itemPartition->setMinnode(partConfigHandler->
      getItemMinNode(util.stringToInt((*params)["SLAVE_INDEX"])));
  itemPartition->initContainers();
  builder.buildTransposeFromFile(itemPartFile, itemPartition);

  // Sets node.
  node->setUserPartition(userPartition);
  node->setItemPartition(itemPartition);
  node->setAlsPartConfigHandler(partConfigHandler);

  string userFeatOutFile = (*params)["LOCAL_DIR"] + "/user_feats_" + (*params)["SLAVE_INDEX"];
  string itemFeatOutFile = (*params)["LOCAL_DIR"] + "/item_feats_" + (*params)["SLAVE_INDEX"];
  node->setUserFeatFile(userFeatOutFile);
  node->setItemFeatFile(itemFeatOutFile);

  int slaveIndex;
  sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slaveIndex);
  node->setPartitionIndex(slaveIndex);

  return node;
}

void AlsMulti2StepFactory::setPartitionConfigHandler(AlsPartitionConfigHandler* handler) {
  partConfigHandler = handler;
}
