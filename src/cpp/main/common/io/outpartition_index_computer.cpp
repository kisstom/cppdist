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
  numNeighbors->resize(partNumNodes);

  outPartitions = new vector<set<int> >();
  outPartitions->resize(partNumNodes);
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
  int partI = 0;
  long numCrossEdge = 0;
  char* line = new char[rowlen];
  long current_row = 0;
  vector<long> edges;


  while (fgets(line, rowlen, inputFile)) {
    if (strcmp(line, "\n") == 0) {
      ++current_row;
      continue;
    }

    if (strlen(line) > 0) {
      line[strlen(line) - 1]= ' ';
    }

    edges.clear();
    util.splitByToken(line, edges);

    (*numNeighbors)[current_row] = (int) edges.size();
    for (int i = 0; i < (int) edges.size(); ++i) {
      partI = getPartitionIndex(edges[i]);
      if (partI >= 0) {
        (*outPartitions)[current_row].insert(partI);
        ++numCrossEdge;
      }
    }

    ++current_row;
  }

  logger_->info("Number of edges between partitions: %ld", numCrossEdge);
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
  countPartitions();

  fclose(inputFile);
  fclose(cfgFile);
}

void OutPartitionIndexComputer::countPartitions() {
  long outPartSize = 0;
  for (int i = 0; i < (int) outPartitions->size(); ++i) {
    //if (i == partIndex) continue;
    outPartSize += (long) (*outPartitions)[i].size();
  }

  logger_->info("Out partition count %ld", outPartSize);
}


vector<set<int> >* OutPartitionIndexComputer::getOutPartitions() {
  return outPartitions;
}

vector<int>* OutPartitionIndexComputer::getNumNeighbors() {
  return numNeighbors;
}



