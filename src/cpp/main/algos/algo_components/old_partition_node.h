#ifndef OLD_PARTITION_NODE_H_
#define OLD_PARTITION_NODE_H_

#include "node.h"
#include "factories/graph_partition_config_handler.h"

class OldPartitionNode: public Node {
public:
  virtual void setPartitionConfigHandler(GraphPartitionConfigHandler*) = 0;
};

#endif  // OLD_PARTITION_NODE_H_
