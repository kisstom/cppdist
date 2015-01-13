#ifndef ALS_NODE_FACTORY_H_
#define ALS_NODE_FACTORY_H_

#include <log4cpp/Category.hh>
#include "../inode_factory.h"
#include "../node_factory_helper.h"
#include "als_partition_config_handler.h"

class AlsNodeFactory : public INodeFactory {
public:
  AlsNodeFactory();
  Node* createNodeFromConfig(unordered_map<string, string>* params);
  void setPartitionConfigHandler(AlsPartitionConfigHandler* handler);
private:
  AlsPartitionConfigHandler* partConfigHandler;
};


#endif  // ALS_NODE_FACTORY_H_
