#ifndef MASTER_FACTORY_H_
#define MASTER_FACTORY_H_

#include "log4cpp/Category.hh"
#include "imaster_factory.h"
#include <tr1/unordered_map>
#include <string>

using std::tr1::unordered_map;
using std::string;

class MasterFactory : public IMasterFactory {
public:
  MasterFactory();
  MasterBase* createMaster(unordered_map<string, string>* params);
private:
  vector<Slave>* readSlaveConfigs(char* cfg, int num_slaves);
  log4cpp::Category* logger_;
};


#endif  // MASTER_FACTORY_H_
