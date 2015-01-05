#include "test_master_factory.h"
#include "../factories/master_factory_helper.h"

TestMasterFactory::TestMasterFactory() {
  logger_ = &log4cpp::Category::getInstance(std::string("TestMasterFactory"));
  slaves_ = NULL;
}

MasterBase* TestMasterFactory::createMaster(unordered_map<string, string>* params) {
  MasterFactoryHelper helper;
  return helper.initMaster(params, slaves_);
}

void TestMasterFactory::setTestSlaveConfig(vector<Slave>* slaves) {
  slaves_ = slaves;
}
