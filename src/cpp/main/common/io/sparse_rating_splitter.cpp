#include "sparse_rating_splitter.h"
#include <cstdio>

SparseRatingSplitter::SparseRatingSplitter(string _outputPrefix,
    long _numEdgePerPart) {
  logger_ = &log4cpp::Category::getInstance(std::string("SparseRatingSplitter"));
  outputPrefix = _outputPrefix;
  numEdgePerPart = _numEdgePerPart;
  partitionCounter = -1;
  actPartition = NULL;
}

void SparseRatingSplitter::process(FILE* input) {
  openNextPartition();

  long from, to;
  double value;

  long numEdges = 0;
  while (fscanf(input, "%ld %ld %lf\n", &from, &to, &value) != EOF) {
    if (numEdges >= numEdgePerPart) {
      fclose(actPartition);
      openNextPartition();
      numEdges = 0;
    }

    fprintf(actPartition, "%ld %ld %lf\n", from, to, value);
    ++numEdges;
  }
}


void SparseRatingSplitter::openNextPartition() {
  ++partitionCounter;
  string actPartName = outputPrefix + util.intToString(partitionCounter);

  actPartition = fopen(actPartName.c_str(), "w");
  if (NULL == actPartition) {
    logger_->info("Error opening partition: %s\n", actPartName.c_str());
  }
}
