#ifndef ALS_PARTITION_CONFIG_HANDLER_H_
#define ALS_PARTITION_CONFIG_HANDLER_H_

#include <vector>
#include <string>
#include <tr1/unordered_map>
#include "log4cpp/Category.hh"
#include "../../../common/util/util.h"
#include "graph_partition_config_handler.h"

using std::vector;
using std::string;
using std::tr1::unordered_map;

class AlsPartitionConfigHandler {
public:
  AlsPartitionConfigHandler();
  void setUserPartitionHandler(GraphPartitionConfigHandler*);
  void setItemPartitionHandler(GraphPartitionConfigHandler*);

  void readItemConfig(string, int);
  void readUserConfig(string, int);

  long getUserMinNode(int);
  long getUserNextMinNode(int);
  long getNumUser(int);
  int getItemPartitionIndex(long);

  long getItemMinNode(int);
  long getItemNextMinNode(int);
  long getNumItem(int);
  int getUserPartitionIndex(long);

  ~AlsPartitionConfigHandler();
private:
  log4cpp::Category* logger_;
  Util util;
  GraphPartitionConfigHandler* userPartitionHandler;
  GraphPartitionConfigHandler* itemPartitionHandler;
};



#endif  // ALS_PARTITION_CONFIG_HANDLER_H_
