#include "als_node_factory.h"

AlsNodeFactory::AlsNodeFactory() {
  partConfigHandler = NULL;
}

Node* AlsNodeFactory::createNodeFromConfig(unordered_map<string, string>* params) {
  return NULL;
}

void AlsNodeFactory::setPartitionConfigHandler(AlsPartitionConfigHandler* handler) {
  partConfigHandler = handler;
}
