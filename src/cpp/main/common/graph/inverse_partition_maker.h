/*
 * inverse_partition_maker.h
 *
 *  Created on: Sep 8, 2014
 *      Author: kisstom
 */

#ifndef INVERSE_PARTITION_MAKER_H_
#define INVERSE_PARTITION_MAKER_H_

#include <string>
#include <vector>

using std::vector;
using std::string;

class InversePartitionMaker {
public:
  InversePartitionMaker(string inputF, string dirPrefix, int numslaves, int rowlen);
  void run();
  void process(FILE*);
private:
  void initBoundFiles();
  void initEdgelistFiles();
  void readPartitionBounds(FILE*);
  int getPartitionIndex(long);
  void closeFiles();
  vector<std::pair<long, long> > partitionBounds;
  vector<FILE*> edgeListPartitions;
  vector<FILE*> edgeListBoundPointers;
  Util util;
  int numslaves, ROWLEN;

  string dirPrefix;
  string inputFileName;
  string edgeListPrefix;
  string boundPrefix;
};


#endif /* INVERSE_PARTITION_MAKER_H_ */
