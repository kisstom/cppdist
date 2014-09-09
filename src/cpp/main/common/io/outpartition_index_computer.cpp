/*
 * outpartition_index_computer.cpp

 *
 *  Created on: 2014.09.09.
 *      Author: kisstom
 */

#include <cstdio>
#include "outpartition_index_computer.h"

OutPartitionIndexComputer::OutPartitionIndexComputer(string input, string cfg, int _numslaves, int _rowlen, long _numnodes) {
  inputPartition = input;
  slaveConfig = cfg;
  numslaves = _numslaves;
  rowlen = _rowlen;
  numnodes = _numnodes;

  numNeighbors = new vector<int>();
  numNeighbors->resize(numnodes);

  outPartitions = new vector<set<long> >();
  outPartitions->resize(numnodes);

  partitionBounds.resize(numslaves);
}

void OutPartitionIndexComputer::readConfig(FILE* slaveryFile) {
  long lowerBound = 0, numNodes = 0, upperBound = 0;
  for (int i = 0; i < numslaves; ++i) {
    fscanf(slaveryFile,"%*d %*s %ld %*ld %ld", &numNodes, &lowerBound);
    upperBound = lowerBound + numNodes;
    partitionBounds.push_back(std::make_pair<long, long>(lowerBound, upperBound));
  }
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
  int partIndex = 0;
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
      partIndex = getPartitionIndex(edges[i]);
      (*outPartitions)[current_row].insert(partIndex);
    }

    ++current_row;
  }
}

void OutPartitionIndexComputer::run() {
  FILE* cfgFile = fopen(slaveConfig.c_str(), "r");
  FILE* inputFile = fopen(inputPartition.c_str(), "r");

  readConfig(cfgFile);
  process(inputFile);

  fclose(inputFile);
  fclose(cfgFile);
}

vector<set<long> >* OutPartitionIndexComputer::getOutPartitions() {
  return outPartitions;
}

vector<int>* OutPartitionIndexComputer::getNumNeighbors() {
  return numNeighbors;
}



