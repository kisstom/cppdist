#include <cstdlib>

#include "master_factory_helper.h"
#include "../master_base/master_base.h"
#include "../master_base/slave.h"
#include "../master_base/two_threaded_master.h"
#include "../master_base/three_threaded_master.h"
#include <exception>

MasterFactoryHelper::MasterFactoryHelper () {
  logger_ = &log4cpp::Category::getInstance(std::string("MasterFactoryHelper"));
}

MasterBase* MasterFactoryHelper::initMaster(unordered_map<string, string>* params,
    vector<Slave>* slaves) {
  MasterBase* master = NULL;
  int master_port, num_slaves;
  long numLine;

  sscanf((*params)["MASTER_PORT"].c_str(), "%d", &master_port);
  sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves);

  if (params->find("MULTI") != params->end()) {
    if ((*params)["MULTI"].compare("MULTICAST") == 0 ||
        (*params)["MULTI"].compare("BROADCAST") == 0) {
      return new ThreeThreadedMaster(master_port, slaves);
    }

    throw std::exception();
  }

  return new TwoThreadedMaster(master_port, slaves);
}
