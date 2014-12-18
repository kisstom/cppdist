#include "algo_base.h"

AlgoBase::AlgoBase(unordered_map<string, string>* _params) {
  logger_ = &log4cpp::Category::getInstance(std::string("AlgoBase"));
  initFromParams(_params);

}

int AlgoBase::getSlaveIndex() {
  return slave_index_;
}

int AlgoBase::getPartitionIndex(long node) {
  for (int i = partition_min_node_.size() - 1; i >= 0; --i) {
    if (partition_min_node_[i] <= node) return i;
  }
  return 0;
}

long AlgoBase::getPartitionStartNode(int part_index) {
  if (part_index < 0) return -1;
  if (part_index >= (int) partition_min_node_.size()) {
    return partition_min_node_[partition_min_node_.size() - 1] + num_nodes_;
  }

  return partition_min_node_[part_index];
}

short AlgoBase::getNumberOfPartitions() {
  return (short) partition_min_node_.size();
}

void AlgoBase::final() {
   node_->final();
   logger_->info("AlgoBase finished.");
}

bool AlgoBase::storeFromBinary(int socket_index) {
  return storeFromBinary_->storeFromIndex(socket_index);
}

void AlgoBase::setNode(Node * node) {
  node_ = node;
}

void AlgoBase::setSocketManager(ISocketManager* manager) {
  socketManager_ = manager;
  senderBuffer_->setSocketManager(manager);
}

void AlgoBase::setMasterSocketManager(MasterSocketManager* manager) {
  masterSocketManager_ = manager;
}

/*void AlgoBase::setClientSocketManager(ClientSocketManager* manager) {
  clientSocketManager_ = manager;
}*/

void AlgoBase::setStoreFromBinary(StoreFromBinary* storeFromBinary) {
  storeFromBinary_ = storeFromBinary;
}

void AlgoBase::setSenderBuffer(SenderBuffer* senderBuffer) {
  senderBuffer_ = senderBuffer;
  node_->setSenderBuffer(senderBuffer);
}

long AlgoBase::getNumberOfPartitionNodes() {
  return num_nodes_;
}

long AlgoBase::getAllNodes() {
  return all_node_;
}

long AlgoBase::getMinnode() {
  return min_node_;
}

AlgoBase::~AlgoBase() {
}
