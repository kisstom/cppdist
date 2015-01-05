#include "master_factory.h"
#include "../master_base/two_threaded_master.h"
#include "../master_base/three_threaded_master.h"
#include "../factories/master_factory_helper.h"
#include <cstdlib>

MasterFactory::MasterFactory() {
  logger_ = &log4cpp::Category::getInstance(std::string("MasterFactory"));
}

MasterBase* MasterFactory::createMaster(unordered_map<string, string>* params) {
  char logfile_name[1024], slavery_cfg[1024];
  int num_slaves;
  strcpy(logfile_name, (*params)["LOGFILE_NAME"].c_str());
  strcpy(slavery_cfg, (*params)["REMOTE_DIR"].c_str());
  strcat(slavery_cfg, (*params)["SLAVERY_CFG"].c_str());
  sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves);

  vector<Slave>* slaves = readSlaveConfigs(slavery_cfg, num_slaves);
  MasterFactoryHelper helper;
  return helper.initMaster(params, slaves);
}

vector<Slave>* MasterFactory::readSlaveConfigs(char* cfg, int num_slaves) {
  vector<Slave>* slaves;
  FILE* slavery_par = fopen(cfg, "r");
  if (slavery_par == NULL)
  {
    // TODO node-oknak is meg kell halniuk
    logger_->error("Error opening %s in master main.", cfg);
    exit(1);
  }
  slaves = new vector<Slave>(num_slaves);
  for (int i = 0; i < num_slaves; ++i)
  {
    fscanf(slavery_par,"%d %*s %ld %*ld %ld", &((*slaves)[i].port), &((*slaves)[i].numNode),  &((*slaves)[i].minNode));
  }
  fclose(slavery_par);
  return slaves;
}
