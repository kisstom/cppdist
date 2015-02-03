#include "als_broadcast_node_factory.h"
#include "../../als_broadcast/als_broadcast_node.h"
#include "../../../common/graph/builder/adjacency_list_builder.h"
#include "../../../common/graph/adjacency_list.h"

AlsBroadcastNodeFactory::AlsBroadcastNodeFactory() {
  partConfigHandler = NULL;
  logger_ = &log4cpp::Category::getInstance(std::string("AlsBroadcastNodeFactory"));
}

Node* AlsBroadcastNodeFactory::createNodeFromConfig(
    unordered_map<string, string>* params) {
  logger_->info("Creating als broadcast node.");
  AlsBroadcastNode* node = new AlsBroadcastNode(params);

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

void AlsBroadcastNodeFactory::setPartitionConfigHandler(AlsPartitionConfigHandler* handler) {
  partConfigHandler = handler;
}
