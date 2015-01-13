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
  sscanf((*params)["NUMLINE"].c_str(), "%ld", &all_node_);
  sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slave_index_);
  sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves_);
  sscanf((*params)["SEND_LIMIT"].c_str(), "%d", &send_limit_);

  slave_port_ = init_slave_port + slave_index_;
}

int AlgoBase::getSlaveIndex() {
  return slave_index_;
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

void AlgoBase::setStoreFromBinary(StoreFromBinary* storeFromBinary) {
  storeFromBinary_ = storeFromBinary;
}

void AlgoBase::setSenderBuffer(SenderBuffer* senderBuffer) {
  senderBuffer_ = senderBuffer;
  node_->setSenderBuffer(senderBuffer);
}

long AlgoBase::getAllNodes() {
  return all_node_;
}

int AlgoBase::getNumSlaves() {
  return num_slaves_;
}

AlgoBase::~AlgoBase() {
}
