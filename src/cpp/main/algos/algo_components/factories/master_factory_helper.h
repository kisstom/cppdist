#ifndef MASTER_FACTORY_HELPER_H_
#define MASTER_FACTORY_HELPER_H_

class MasterBase;
class Slave;

#include <tr1/unordered_map>
#include <string>
#include <vector>
#include "log4cpp/Category.hh"

using std::tr1::unordered_map;
using std::string;
using std::vector;

class MasterFactoryHelper {
public:
  MasterFactoryHelper();
  MasterBase* initMaster(unordered_map<string, string>* params,
      vector<Slave>* slaves);
private:
  log4cpp::Category* logger_;
};


#endif  // MASTER_FACTORY_HELPER_H_
