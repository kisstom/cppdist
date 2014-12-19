#include "test_master_factory.h"

TestMasterFactory::TestMasterFactory() {
  logger_ = &log4cpp::Category::getInstance(std::string("TestMasterFactory"));
  slaves_ = NULL;
}

MasterBase* TestMasterFactory::createMaster(unordered_map<string, string>* params) {
  int master_port, num_slaves;
  long numLine;

  sscanf((*params)["MASTER_PORT"].c_str(), "%d", &master_port);
  sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves);
  sscanf((*params)["NUMLINE"].c_str(), "%ld", &numLine);

  //MasterBase* master = new MasterBase(master_port, slaves_, numLine);
  return NULL;
}

void TestMasterFactory::setTestSlaveConfig(vector<Slave>* slaves) {
  slaves_ = slaves;
}
