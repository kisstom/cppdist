#ifndef GRAPH_PARTITION_CONFIG_HANDLER_H_
#define GRAPH_PARTITION_CONFIG_HANDLER_H_

#include <vector>
#include <string>
#include <tr1/unordered_map>
#include "log4cpp/Category.hh"
#include "../../../common/util/util.h"

using std::vector;
using std::string;
using std::tr1::unordered_map;

class GraphPartitionConfigHandler {
public:
  GraphPartitionConfigHandler();
  void readSlaveConfig(string, int);
  void setPartitionMinNodes(vector<long>*);
  void setPartitionNumNodes(vector<long>*);

  long getMinNode(int) const;
  long getNextMinNode(int) const;
  long getNumNode(int) const;
  int getPartitionIndex(long) const;

  ~GraphPartitionConfigHandler();
private:
  log4cpp::Category* logger_;
  Util util;
  vector<long>* partitionMinNodes;
  vector<long>* partitionNumNodes;
};


#endif  // GRAPH_PARTITION_CONFIG_HANDLER_H_
