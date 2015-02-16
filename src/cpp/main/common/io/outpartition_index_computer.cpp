/*
 * outpartition_index_computer.cpp

 *
 *  Created on: 2014.09.09.
 *      Author: kisstom
 */

#include <cstdio>
#include "outpartition_index_computer.h"

OutPartitionIndexComputer::OutPartitionIndexComputer(string input, string cfg, int _numslaves, int _rowlen, int _partIndex) {
  inputPartition = input;
  slaveConfig = cfg;
  numslaves = _numslaves;
  rowlen = _rowlen;
  partIndex = _partIndex;
  logger_ = &log4cpp::Category::getInstance(std::string("OutPartitionIndexComputer"));
}

void OutPartitionIndexComputer::readConfig(FILE* slaveryFile) {
  logger_->info("Reading config");
  long lowerBound = 0, numNodes = 0, upperBound = 0, partNumNodes = 0;
  for (int i = 0; i < numslaves; ++i) {
    fscanf(slaveryFile,"%ld %*ld %ld", &numNodes, &lowerBound);
    upperBound = lowerBound + numNodes;
    partitionBounds.push_back(std::make_pair<long, long>(lowerBound, upperBound));
    if (i == partIndex) {
      partNumNodes = numNodes;
    }
  }

  numNeighbors = new vector<int>();
  numNeighbors->resize(partNumNodes);

  outPartitions = new vector<short*>();
  outPartitions->resize(partNumNodes);
  logger_->info("Out partitions size %d", partNumNodes);
}

int OutPartitionIndexComputer::getPartitionIndex(long node) {
  for (int i = 0; i < (int) partitionBounds.size(); ++i) {
    if (partitionBounds[i].first <= node && node < partitionBounds[i].second) {
      return i;
    }
  }
  return -1;
}

void OutPartitionIndexComputer::process(FILE* inputFile) {
  logger_->info("Processing out partition indices.");
  int partI = 0;
  long numCrossEdge = 0;
  long outPartSize = 0;
  long allOutPartSize = 0;
  char* line = new char[rowlen];
  long current_row = 0;
  vector<long> edges;
  set<short> outIndices;

  bool shouldUseNetwork = false;
  int numShouldUse = 0;
  while (fgets(line, rowlen, inputFile)) {
    if (strcmp(line, "\n") == 0) {
      (*outPartitions)[current_row] = new short[1];
      (*outPartitions)[current_row][0] = 0;
      ++current_row;
      continue;
    }

    if (strlen(line) > 0) {
      line[strlen(line) - 1]= '\0';
    }

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

    short* indices = new short[outIndices.size() + 1];
    indices[0] = outIndices.size();
    allOutPartSize += (long) outIndices.size();

    short count = 1;
    for (set<short>::const_iterator it = outIndices.begin(); it != outIndices.end(); ++it) {
      indices[count] = *it;
      ++count;
      if (*it != partIndex) {
        ++outPartSize;
      }
    }
    (*outPartitions)[current_row] = indices;

    ++current_row;
  }

  logger_->info("Num should use: %d", numShouldUse);
  logger_->info("Number of edges between partitions: %ld", numCrossEdge);
  logger_->info("Out partition count %ld", outPartSize);
  logger_->info("All partition count %ld", allOutPartSize);
}

void OutPartitionIndexComputer::run() {
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

void OutPartitionIndexComputer::countPartitions() {
  long outPartSize = 0;
  short size = -1;
  for (int i = 0; i < (int) outPartitions->size(); ++i) {
    size = (*outPartitions)[i][0];
    for (short c = 0; c < size; ++c) {
      if ((*outPartitions)[i][c + 1] != partIndex) {
        ++outPartSize;
      }
    }
  }

  logger_->info("Out partition count %ld", outPartSize);
}

void OutPartitionIndexComputer::flushAsEdgelistContainer(FILE* outfile) {
  int size = -1;
  for (long i = 0; i < (long) outPartitions->size(); ++i) {
    size = (*outPartitions)[i][0];
    for (int c = 0; c < size; ++c) {
      if (c == 0) {
        fprintf (outfile, "%hd", (*outPartitions)[i][c + 1]);
      } else {
        fprintf (outfile, " %hd", (*outPartitions)[i][c + 1]);
      }
    }

    fprintf(outfile, "\n");
  }
}

void OutPartitionIndexComputer::flushNeighborsFile(FILE* neighborsFile) {
  for (long i = 0; i < (long) numNeighbors->size(); ++i) {
    fprintf(neighborsFile, "%d\n", numNeighbors->at(i));
  }
}

vector<short*>* OutPartitionIndexComputer::getOutPartitions() {
  return outPartitions;
}

vector<int>* OutPartitionIndexComputer::getNumNeighbors() {
  return numNeighbors;
}



