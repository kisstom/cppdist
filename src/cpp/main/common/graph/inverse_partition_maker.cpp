/*
 * inverse_partition_maker.cpp
 *
 *  Created on: Sep 8, 2014
 *      Author: kisstom
 */

#include <cstdio>
#include <cstring>
#include <sstream>

#include "../util/util.h"
#include "inverse_partition_maker.h"

using std::stringstream;

InversePartitionMaker::InversePartitionMaker(string _inputFile, string _dirPrefix,
    int _numslaves, int _rowLen) {
  inputFileName = _inputFile;
  dirPrefix = _dirPrefix;
  numslaves = _numslaves;
  ROWLEN = _rowLen;
}

void InversePartitionMaker::readPartitionBounds(FILE* slaveryFile) {
  long lowerBound = 0, numNodes = 0, upperBound = 0;
  for (int i = 0; i < numslaves; ++i) {
    fscanf(slaveryFile,"%*d %*s %ld %ld %*ld", &lowerBound,  &numNodes);
    upperBound = lowerBound + numNodes;
    partitionBounds.push_back(std::make_pair<int, int>(lowerBound, upperBound));
  }
}

int InversePartitionMaker::getPartitionIndex(long node) {
  for (int i = 0; i < partitionBounds.size(); ++i) {
    if (partitionBounds[i].first <= node && node < partitionBounds[i].second) {
      return i;
    }
  }
  return -1;
}

void InversePartitionMaker::initBoundFiles() {
  for (int i = 0; i < numslaves; ++i) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    ss << dirPrefix << "/" << "part_" << i << "/" + boundPrefix;
    edgeListBoundPointers[i] = fopen(ss.str().c_str(), "w");

    if (NULL == edgeListBoundPointers[i]) {
      fprintf(stderr, "Error opening file: %s\n", ss.str().c_str());
    }
  }
}

void InversePartitionMaker::initEdgelistFiles() {
  for (int i = 0; i < numslaves; ++i) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    ss << dirPrefix << "/" << "part_" << i << "/" + edgeListPrefix;
    edgeListPartitions[i] = fopen(ss.str().c_str(), "w");

    if (NULL == edgeListPartitions[i]) {
      fprintf(stderr, "Error opening file: %s\n", ss.str().c_str());
    }
  }
}

void InversePartitionMaker::closeFiles() {
  for (int i = 0; i < numslaves; ++i) {
    fclose(edgeListBoundPointers[i]);
    fclose(edgeListPartitions[i]);
  }
}

void InversePartitionMaker::run() {
  FILE* inputFile = fopen(inputFileName.c_str(), "r");
  if (NULL == inputFile) {
    fprintf(stderr, "Error opening file: %s\n", inputFileName.c_str());
  }

  initBoundFiles();
  initEdgelistFiles();

  process(inputFile);
  fclose(inputFile);

  closeFiles();
}

void InversePartitionMaker::process(FILE* inputFile) {
  int partIndex = 0;
  char* line = new char[ROWLEN];
  long current_row = 0;
  vector<long> edges;
  vector<int> edgesToPartition;

  edgesToPartition.resize(numslaves);
  for (int i = 0; i < numslaves; ++i) {
    edgesToPartition[i] = 0;
  }

  while (fgets(line, ROWLEN, inputFile)) {
    if (strcmp(line, "\n") == 0) {
      ++current_row;
      continue;
    }

    if (strlen(line) > 0) {
      line[strlen(line) - 1]= ' ';
    }

    edges.clear();
    util.splitByToken(line, edges);

    for (int i = 0; i < (int) edges.size(); ++i) {
      partIndex = getPartitionIndex(edges[i]);
      if (edgesToPartition[partIndex] == 0) {
        fprintf(edgeListPartitions[partIndex], "%ld\n", edges[i]);
      } else {
        fprintf(edgeListPartitions[partIndex], " %ld\n", edges[i]);
      }
      ++edgesToPartition[partIndex];
    }

    for (int i = 0; i < numslaves; ++i) {
      fprintf(edgeListBoundPointers[i], "%ld %ld\n", current_row, edgesToPartition[i]);
    }

    ++current_row;
  }
}
