#ifndef MASTER_FACTORY_H_
#define MASTER_FACTORY_H_

#include "log4cpp/Category.hh"
#include "imaster_factory.h"
#include "../master_base/slave.h"
#include "../../../common/util/util.h"

#include <tr1/unordered_map>
#include <string>
#include <vector>


using std::tr1::unordered_map;
using std::string;
using std::vector;

class MasterFactory : public IMasterFactory {
public:
  MasterFactory();
  MasterBase* createMaster(unordered_map<string, string>* params);
private:
  vector<Slave>* initSlaves(int initSlavePort, int num_slaves);
  log4cpp::Category* logger_;
  Util util;
};


#endif  // MASTER_FACTORY_H_
