#ifndef IMASTER_FACTORY_H_
#define IMASTER_FACTORY_H_

//#include "../master_base/master_base.h"
#include <string>
#include <tr1/unordered_map>

using std::string;
using std::tr1::unordered_map;

class MasterBase;

class IMasterFactory {
public:
  virtual MasterBase* createMaster(unordered_map<string, string>* p) = 0;
  virtual ~IMasterFactory() {}
};


#endif  // IMASTER_FACTORY_H_
