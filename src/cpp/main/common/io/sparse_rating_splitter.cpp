#include "sparse_rating_splitter.h"
#include <cstdio>

SparseRatingSplitter::SparseRatingSplitter(string _outputPrefix,
    long _numEdgePerPart) {
  logger_ = &log4cpp::Category::getInstance(std::string("SparseRatingSplitter"));
  outputPrefix = _outputPrefix;
  numEdgePerPart = _numEdgePerPart;
  partitionCounter = -1;
  actPartition = NULL;
  partiConfig = NULL;
}

void SparseRatingSplitter::setPartiConfig(FILE* _partiConfig) {
  partiConfig = _partiConfig;
}

void SparseRatingSplitter::process(FILE* input) {
  openNextPartition();

  long from, to, lastFromId;
  double value;
  long numEdges = 0;
  long lastMinNode = 0;

  while (fscanf(input, "%ld %ld %lf\n", &from, &to, &value) != EOF) {
    if (numEdges >= numEdgePerPart && lastFromId != from) {
      fprintf(partiConfig, "%ld %ld %ld\n", from - lastMinNode, numEdges, lastMinNode);
      fclose(actPartition);
      openNextPartition();
      numEdges = 0;
      lastMinNode = from;
    }

    fprintf(actPartition, "%ld %ld %lf\n", from, to, value);
    ++numEdges;
    lastFromId = from;
  }

  fprintf(partiConfig, "%ld %ld %ld\n", from - lastMinNode + 1, numEdges, lastMinNode);
}

void SparseRatingSplitter::processTranspose(FILE* input) {
  openNextPartition();

  long from, to, lastToId;
  double value;
  long numEdges = 0;
  long lastMinNode = 0;

  while (fscanf(input, "%ld %ld %lf\n", &from, &to, &value) != EOF) {
    if (numEdges >= numEdgePerPart && lastToId != to) {
      fprintf(partiConfig, "%ld %ld %ld\n", to - lastMinNode, numEdges, lastMinNode);
      fclose(actPartition);
      openNextPartition();
      numEdges = 0;
      lastMinNode = to;
    }

    fprintf(actPartition, "%ld %ld %lf\n", from, to, value);
    ++numEdges;
    lastToId = to;
  }

  fprintf(partiConfig, "%ld %ld %ld\n", from - lastMinNode + 1, numEdges, lastMinNode);
}

void SparseRatingSplitter::openNextPartition() {
  ++partitionCounter;
  string actPartName = outputPrefix + util.intToString(partitionCounter);

  actPartition = fopen(actPartName.c_str(), "w");
  if (NULL == actPartition) {
    logger_->info("Error opening partition: %s\n", actPartName.c_str());
  }
}
