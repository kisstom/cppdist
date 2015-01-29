#ifndef OLD_PARTITION_NODE_FACTORY_H_
#define OLD_PARTITION_NODE_FACTORY_H_

#include "../inode_factory.h"
#include "graph_partition_config_handler.h"
#include "../../../common/graph/edgelist_container.h"

class OldPartitionNodeFactory : public INodeFactory {
public:
  virtual void setEdgelistContainer(EdgelistContainer*) = 0;
  virtual void setPartConfHandler(GraphPartitionConfigHandler*) = 0;
};


#endif  // OLD_PARTITION_NODE_FACTORY_H_
