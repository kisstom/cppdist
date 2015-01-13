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

  long getMinNode(int);
  long getNextMinNode(int);
  long getNumNode(int);
  int getPartitionIndex(long);

  ~GraphPartitionConfigHandler();
private:
  log4cpp::Category* logger_;
  Util util;
  vector<long>* partitionMinNodes;
  vector<long>* partitionNumNodes;
};


#endif  // GRAPH_PARTITION_CONFIG_HANDLER_H_
