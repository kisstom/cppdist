/*
 * inverse_partition_maker.cpp
 *
 *  Created on: Sep 8, 2014
 *      Author: kisstom
 */

#include <cstdio>
#include <cstring>
#include <sstream>

#include "inverse_partition_maker.h"

using std::stringstream;

InversePartitionMaker::InversePartitionMaker(string _inputFile, string _dirPrefix, string _slaveryFile,
    int _numslaves, int _rowLen) {
  inputFileName = _inputFile;
  slaveryFile = _slaveryFile;
  dirPrefix = _dirPrefix;
  numslaves = _numslaves;
  ROWLEN = _rowLen;

  edgeListPrefix = "edges.txt";
  boundPrefix = "bound.txt";
}

void InversePartitionMaker::readPartitionBounds(FILE* slaveryFile) {
  long lowerBound = 0, numNodes = 0, upperBound = 0;
  for (int i = 0; i < numslaves; ++i) {
    fscanf(slaveryFile,"%*d %*s %ld %*ld %ld", &numNodes, &lowerBound);
    upperBound = lowerBound + numNodes;
    partitionBounds.push_back(std::make_pair<long, long>(lowerBound, upperBound));
    printf("%ld %ld\n", lowerBound, upperBound);
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
  edgeListBoundPointers.resize(numslaves);

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
  edgeListPartitions.resize(numslaves);

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

  FILE* slaver = fopen(slaveryFile.c_str(),  "r");
  if (slaver == NULL) {
    fprintf(stderr, "Error opening file %s\n", slaveryFile.c_str());
  }

  readPartitionBounds(slaver);
  fclose(slaver);

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
  vector<long> edgesToPartition;
  vector<long> prevEdgesToPartition;

  edgesToPartition.resize(numslaves);
  prevEdgesToPartition.resize(numslaves);
  for (int i = 0; i < numslaves; ++i) {
    edgesToPartition[i] = 0;
    prevEdgesToPartition[i] = 0;
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

    for (int i = 0; i < numslaves; ++i) {
      prevEdgesToPartition[i] = edgesToPartition[i];
    }

    for (int i = 0; i < (int) edges.size(); ++i) {
      partIndex = getPartitionIndex(edges[i]);
      if (-1 == partIndex) {
        fprintf(stderr, "Partition not found: %ld\n", edges[i]);
        continue;
      }
      fprintf(edgeListPartitions[partIndex], "%ld\n", edges[i]);
      ++edgesToPartition[partIndex];
    }

    for (int i = 0; i < numslaves; ++i) {
      if (edgesToPartition[i] > prevEdgesToPartition[i]) {
        fprintf(edgeListBoundPointers[i], "%ld %ld\n", current_row, edgesToPartition[i]);
      }
    }

    ++current_row;
  }
}
