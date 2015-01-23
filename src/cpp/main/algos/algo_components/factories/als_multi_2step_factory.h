#ifndef ALS_MULTI_2STEP_FACTORY_H_
#define ALS_MULTI_2STEP_FACTORY_H_

#include <log4cpp/Category.hh>
#include "../inode_factory.h"
#include "../../../common/util/util.h"
#include "als_partition_config_handler.h"

class AlsMulti2StepFactory : public INodeFactory {
public:
  AlsMulti2StepFactory();
  Node* createNodeFromConfig(unordered_map<string, string>* params);
  void setPartitionConfigHandler(AlsPartitionConfigHandler* handler);
private:
  Util util;
  AlsPartitionConfigHandler* partConfigHandler;
  log4cpp::Category* logger_;
};

#endif  // ALS_MULTI_2STEP_FACTORY_H_
