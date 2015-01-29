#ifndef ALGO_TEST_UTIL_H_
#define ALGO_TEST_UTIL_H_

#include <string>
#include <vector>
#include <list>

using std::string;
using std::vector;

#include "../../main/algos/algo_components/factories/old_partition_node_factory.h"

class AlgoTestUtil {
public:
  void createFactoriesFromPart(vector<vector<string> >,vector<OldPartitionNodeFactory*>*);
  void setFingerPrintStarts(vector<list<long*> >*,
      vector<string> part, int pathLen, int numPathes, long minnode);
};

#endif  // ALGO_TEST_UTIL_H_
