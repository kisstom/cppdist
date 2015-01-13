#include "als_partition_config_handler.h"

AlsPartitionConfigHandler::AlsPartitionConfigHandler() {
  logger_ = &log4cpp::Category::getInstance(std::string("AlsPartitionConfigHandler"));
  userPartitionHandler = new GraphPartitionConfigHandler;
  itemPartitionHandler = new GraphPartitionConfigHandler;
}

AlsPartitionConfigHandler::~AlsPartitionConfigHandler() {
  delete userPartitionHandler;
  delete itemPartitionHandler;
}

void AlsPartitionConfigHandler::
setUserPartitionHandler(GraphPartitionConfigHandler* _userPartitionHandler) {
  userPartitionHandler = _userPartitionHandler;
}

void AlsPartitionConfigHandler::
setItemPartitionHandler(GraphPartitionConfigHandler* _itemPartitionHandler) {
  itemPartitionHandler = _itemPartitionHandler;
}

long AlsPartitionConfigHandler::getUserMinNode(int index) {
  return userPartitionHandler->getMinNode(index);
}

long AlsPartitionConfigHandler::getUserNextMinNode(int index) {
  return userPartitionHandler->getNextMinNode(index);
}

long AlsPartitionConfigHandler::getNumUser(int index) {
  return userPartitionHandler->getNumNode(index);
}

int AlsPartitionConfigHandler::getUserPartitionIndex(long key) {
  return userPartitionHandler->getPartitionIndex(key);
}

long AlsPartitionConfigHandler::getItemMinNode(int index) {
  return itemPartitionHandler->getMinNode(index);
}

long AlsPartitionConfigHandler::getItemNextMinNode(int index) {
  return itemPartitionHandler->getMinNode(index);
}

long AlsPartitionConfigHandler::getNumItem(int index) {
  return itemPartitionHandler->getMinNode(index);
}

int AlsPartitionConfigHandler::getItemPartitionIndex(long key) {
  return itemPartitionHandler->getPartitionIndex(key);
}

void AlsPartitionConfigHandler::readItemConfig(string cfg, int index) {
  itemPartitionHandler->readSlaveConfig(cfg, index);
}

void AlsPartitionConfigHandler::readUserConfig(string cfg, int index) {
  userPartitionHandler->readSlaveConfig(cfg, index);
}
