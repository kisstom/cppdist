#ifndef SPARSE_RATING_SPLITTER_H_
#define SPARSE_RATING_SPLITTER_H_

#include "../util/util.h"
#include <string>
#include <log4cpp/Category.hh>

using std::string;

class SparseRatingSplitter {
public:
  SparseRatingSplitter(string, long);
  void process(FILE*);
private:
  void openNextPartition();

  FILE* actPartition;
  string outputPrefix;
  long numEdgePerPart;
  int partitionCounter;
  Util util;
  log4cpp::Category* logger_;
};


#endif  // SPARSE_RATING_SPLITTER_H_
