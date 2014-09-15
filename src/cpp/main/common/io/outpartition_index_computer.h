/*
 * outpartition_index_computer.h
 *
 *  Created on: 2014.09.09.
 *      Author: kisstom
 */

#ifndef OUTPARTITION_INDEX_COMPUTER_H_
#define OUTPARTITION_INDEX_COMPUTER_H_

#include <string>
#include <vector>
#include <set>
#include "../util/util.h"
#include <log4cpp/Category.hh>

using std::vector;
using std::string;
using std::set;

class OutPartitionIndexComputer {
public:
  OutPartitionIndexComputer(string, string, int, int, int);
  void readConfig(FILE*);
  void process(FILE*);
  void run();
  vector<set<int> >* getOutPartitions();
  vector<int>* getNumNeighbors();
  int getPartitionIndex(long node);
  void countPartitions();
private:
  vector<int>* numNeighbors;
  vector<set<int> >* outPartitions;
  vector<std::pair<long, long> > partitionBounds;

  string inputPartition;
  string slaveConfig;
  Util util;
  int partIndex;
  int numslaves;
  int rowlen;
  log4cpp::Category* logger_;
};


#endif /* OUTPARTITION_INDEX_COMPUTER_H_ */
