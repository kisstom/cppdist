#ifndef ALS_BROADCAST_NODE_FACTORY_H_
#define ALS_BROADCAST_NODE_FACTORY_H_

#include <log4cpp/Category.hh>
#include "../inode_factory.h"
#include "../../../common/util/util.h"
#include "als_partition_config_handler.h"

class AlsBroadcastNodeFactory : public INodeFactory {
public:
  AlsBroadcastNodeFactory();
  Node* createNodeFromConfig(unordered_map<string, string>* params);
  void setPartitionConfigHandler(AlsPartitionConfigHandler* handler);
private:
  Util util;
  AlsPartitionConfigHandler* partConfigHandler;
  log4cpp::Category* logger_;
};

#endif  // ALS_BROADCAST_NODE_FACTORY_H_
