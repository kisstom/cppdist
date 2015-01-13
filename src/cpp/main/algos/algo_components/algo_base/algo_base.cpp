#include "algo_base.h"

AlgoBase::AlgoBase(unordered_map<string, string>* _params) {
  logger_ = &log4cpp::Category::getInstance(std::string("AlgoBase"));
  initFromParams(_params);

  socketManager_ = NULL;
  masterSocketManager_ = NULL;
  senderBuffer_ = NULL;
  node_ = NULL;
  storeFromBinary_ = NULL;
}

void AlgoBase::initFromParams(unordered_map<string, string>* params) {
  int init_slave_port;
  strcpy(master_host_, (*params)["MASTER_HOST"].c_str());

  sscanf((*params)["MASTER_PORT"].c_str(), "%d", &master_port_);
  sscanf((*params)["INIT_SLAVE_PORT"].c_str(), "%d", &init_slave_port);

  //sscanf((*params)["MIN_NODE"].c_str(), "%ld", &min_node_);

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &all_node_);
  //sscanf((*params)["NUM_NODES"].c_str(), "%ld", &num_nodes_);

  sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slave_index_);
  sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves_);
  sscanf((*params)["SEND_LIMIT"].c_str(), "%d", &send_limit_);

  slave_port_ = init_slave_port + slave_index_;
}

int AlgoBase::getSlaveIndex() {
  return slave_index_;
}

/*int AlgoBase::getPartitionIndex(long node) {
  for (int i = partition_min_node_->size() - 1; i >= 0; --i) {
    if ((*partition_min_node_)[i] <= node) return i;
  }
  return 0;
}

long AlgoBase::getPartitionStartNode(int part_index) {
  if (part_index < 0) return -1;
  if (part_index >= (int) partition_min_node_->size()) {
    return (*partition_min_node_)[partition_min_node_->size() - 1] + num_nodes_;
  }

  return (*partition_min_node_)[part_index];
}

short AlgoBase::getNumberOfPartitions() {
  return (short) partition_min_node_->size();
}*/

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

void AlgoBase::setStoreFromBinary(StoreFromBinary* storeFromBinary) {
  storeFromBinary_ = storeFromBinary;
}

void AlgoBase::setSenderBuffer(SenderBuffer* senderBuffer) {
  senderBuffer_ = senderBuffer;
  node_->setSenderBuffer(senderBuffer);
}
/*void AlgoBase::setPartitionMinNodes(vector<long>* partitionMinNode) {
  partition_min_node_ = partitionMinNode;
}

long AlgoBase::getNumberOfPartitionNodes() {
  return num_nodes_;
}*/

long AlgoBase::getAllNodes() {
  return all_node_;
}

int AlgoBase::getNumSlaves() {
  return num_slaves_;
}


/*long AlgoBase::getMinnode() {
  return min_node_;
}*/

AlgoBase::~AlgoBase() {
}
