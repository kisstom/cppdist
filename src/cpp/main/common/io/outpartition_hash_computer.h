#ifndef OUTPARTITION_HASH_COMPUTER_H_
#define OUTPARTITION_HASH_COMPUTER_H_

#include <string>
#include <vector>
#include "../util/util.h"
#include "../components/socket/multicast_helper.h"
#include <log4cpp/Category.hh>

using std::string;
using std::vector;

class OutpartitionHashComputer {
public:
  OutpartitionHashComputer(string, string, int, int, int);
  void readConfig(FILE*);
  void process(FILE*);
  void run();

  vector<short>* getOutPartitionHashes();
  vector<bool>* getUpdateFlags();
  vector<int>* getNumNeighbors();
  int getPartitionIndex(long node);
private:
  vector<short>* outPartitionHashes;
  vector<bool>* updateFlags;
  vector<int>* numNeighbors;
  vector<std::pair<long, long> > partitionBounds;

  string inputPartition;
  string slaveConfig;
  Util util;
  int partIndex;
  int numslaves;
  int rowlen;

  MulticastHelper multicastHelper;
  log4cpp::Category* logger_;
};



#endif  // OUTPARTITION_HASH_COMPUTER_H_
