#include "outpartition_hash_computer.h"
#include <set>

using std::set;

OutpartitionHashComputer::OutpartitionHashComputer(
    string input, string cfg, int _numslaves, int _rowlen, int _partIndex): multicastHelper(partIndex) {
  inputPartition = input;
  slaveConfig = cfg;
  numslaves = _numslaves;
  rowlen = _rowlen;
  partIndex = _partIndex;
  logger_ = &log4cpp::Category::getInstance(std::string("OutpartitionHashComputer"));
  outPartitionHashes = NULL;
  updateFlags = NULL;
}

void OutpartitionHashComputer::readConfig(FILE* slaveryFile) {
  logger_->info("Reading config");
  long lowerBound = 0, numNodes = 0, upperBound = 0, partNumNodes = 0;
  for (int i = 0; i < numslaves; ++i) {
    fscanf(slaveryFile,"%*d %*s %ld %*ld %ld", &numNodes, &lowerBound);
    upperBound = lowerBound + numNodes;
    partitionBounds.push_back(std::make_pair<long, long>(lowerBound, upperBound));
    if (i == partIndex) {
      partNumNodes = numNodes;
    }
  }

  numNeighbors = new vector<int>();
  numNeighbors->resize(partNumNodes, 0);

  outPartitionHashes = new vector<short>();
  outPartitionHashes->resize(partNumNodes);

  updateFlags = new vector<bool>();
  updateFlags->resize(partNumNodes, false);
  logger_->info("Out partitions size %d", partNumNodes);
}

int OutpartitionHashComputer::getPartitionIndex(long node) {
  for (int i = 0; i < (int) partitionBounds.size(); ++i) {
    if (partitionBounds[i].first <= node && node < partitionBounds[i].second) {
      return i;
    }
  }
  return -1;
}

void OutpartitionHashComputer::run() {
  FILE* cfgFile = fopen(slaveConfig.c_str(), "r");
  FILE* inputFile = fopen(inputPartition.c_str(), "r");
  if (NULL == cfgFile) {
    logger_->info("Error opening cfg file %s", slaveConfig.c_str());
  }

  if (NULL == inputFile) {
    logger_->info("Error opening input file %s", inputPartition.c_str());
  }

  readConfig(cfgFile);
  process(inputFile);

  fclose(inputFile);
  fclose(cfgFile);
}

void OutpartitionHashComputer::process(FILE* inputFile) {
  logger_->info("Processing out partition indices.");
  int partI = 0;
  long numCrossEdge = 0;
  long outPartSize = 0;
  char* line = new char[rowlen];
  long current_row = 0;
  vector<long> edges;
  set<short> outIndices;
  short hashId;

  bool shouldUseNetwork = false;
  bool shouldUpdate = false;
  int numShouldUse = 0;

  while (fgets(line, rowlen, inputFile)) {
    if (strcmp(line, "\n") == 0) {
      (*outPartitionHashes)[current_row] = -1;
      ++current_row;
      continue;
    }

    if (strlen(line) > 0) {
      line[strlen(line) - 1]= '\0';
    }

    shouldUpdate = false;
    shouldUseNetwork = false;
    util.split(line, edges);

    (*numNeighbors)[current_row] = (int) edges.size();
    outIndices.clear();
    for (int i = 0; i < (int) edges.size(); ++i) {
      partI = getPartitionIndex(edges[i]);
      if (partI >= 0) {
        outIndices.insert(partI);
        if (partI != partIndex) {
          ++numCrossEdge;
          shouldUseNetwork = true;
        }
      }
    }

    if (shouldUseNetwork) ++numShouldUse;
    hashId = multicastHelper.publishHashId(outIndices, &shouldUpdate);

    (*outPartitionHashes)[current_row] = hashId;
    (*updateFlags)[current_row] = shouldUpdate;

    ++current_row;
  }

  logger_->info("Num should use: %d", numShouldUse);
  logger_->info("Number of edges between partitions: %ld", numCrossEdge);
  logger_->info("Out partition count %ld", outPartSize);
}

vector<short>* OutpartitionHashComputer::getOutPartitionHashes() {
  return outPartitionHashes;
}

vector<bool>* OutpartitionHashComputer::getUpdateFlags() {
  return updateFlags;
}

vector<int>* OutpartitionHashComputer::getNumNeighbors() {
  return numNeighbors;
}


