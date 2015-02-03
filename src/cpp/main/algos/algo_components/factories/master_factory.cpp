#include "master_factory.h"
#include "../master_base/two_threaded_master.h"
#include "../master_base/three_threaded_master.h"
#include "../factories/master_factory_helper.h"
#include <cstdlib>

MasterFactory::MasterFactory() {
  logger_ = &log4cpp::Category::getInstance(std::string("MasterFactory"));
}

MasterBase* MasterFactory::createMaster(unordered_map<string, string>* params) {
  int num_slaves = util.stringToInt((*params)["NUM_SLAVES"]);
  int initSlavePort = util.stringToInt((*params)["INIT_SLAVE_PORT"]);

  vector<Slave>* slaves = initSlaves(initSlavePort, num_slaves);
  MasterFactoryHelper helper;
  return helper.initMaster(params, slaves);
}

vector<Slave>* MasterFactory::initSlaves(int initSlavePort, int num_slaves) {
  vector<Slave>* slaves = new vector<Slave>(num_slaves);
  int port = initSlavePort;
  for (int i = 0; i < num_slaves; ++i) {
    (*slaves)[i].port = port++;
    (*slaves)[i].socket = NULL;
  }

  return slaves;
}
