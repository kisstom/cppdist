#ifndef GRAPH_SLAVE_CONFIG_HANDLER_H_
#define GRAPH_SLAVE_CONFIG_HANDLER_H_

#include <vector>
#include <string>
#include <unordered_map>
#include "log4cpp/Category.hh"
#include "../../../common/util/util.h"

using std::vector;
using std::string;
using std::tr1::unordered_map;

class GraphSlaveConfigReader {
public:
  GraphSlaveConfigReader();
  void readSlaveConfig(unordered_map<string, string>* params);
  long getMinNodeFromParams(unordered_map<string, string>* params);
  long getNextMinNodeFromParams(unordered_map<string, string>* params);
  long getNumNodeFromParams(unordered_map<string, string>* params);
  ~GraphSlaveConfigReader();
private:
  log4cpp::Category* logger_;
  Util util;
  vector<long>* partitionMinNodes;
  vector<long>* partitionNumNodes;
};


#endif  // GRAPH_SLAVE_CONFIG_HANDLER_H_
