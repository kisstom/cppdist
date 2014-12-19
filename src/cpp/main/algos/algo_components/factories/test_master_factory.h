#ifndef TEST_MASTER_FACTORY_H_
#define TEST_MASTER_FACTORY_H_

#include "imaster_factory.h"
#include "../master_base/master_base.h"
#include <vector>
#include "log4cpp/Category.hh"

using std::vector;

class TestMasterFactory: public IMasterFactory {
public:
  TestMasterFactory();
  MasterBase* createMaster(unordered_map<string, string>*);

  void setTestSlaveConfig(vector<Slave>* slaves);
private:
  vector<Slave>* slaves_;
  log4cpp::Category* logger_;
};

#endif  // TEST_MASTER_FACTORY_H_
