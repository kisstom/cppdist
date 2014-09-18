/*
 * part_index_reindexer.cpp
 *
 *  Created on: 2014.09.18.
 *      Author: kisstom
 */

#include "shuffle_reindexer.h"
#include <stdio.h>

ShuffleReindexer::ShuffleReindexer(string _inputGraphFile,
    string _outputGraphFile, string _shuffleIndicesFile) {
  inputGraphFile = _inputGraphFile;
  outputGraphFile = _outputGraphFile;
  shuffleIndicesFile = _shuffleIndicesFile;

  rowlen = 15000000;
}

void ShuffleReindexer::init() {
  inputGraph = fopen(inputGraphFile.c_str(), "r");
  outputGraph = fopen(outputGraphFile.c_str(), "w");
  shuffleIndices = fopen(shuffleIndicesFile.c_str(), "r");

  if (NULL == inputGraph)  {
    fprintf(stderr, "Error opening file: %s.\n", inputGraphFile.c_str());
  }

  if (NULL == outputGraph)  {
    fprintf(stderr, "Error opening file: %s.\n", outputGraphFile.c_str());
  }

  if (NULL == shuffleIndices)  {
    fprintf(stderr, "Error opening file: %s.\n", shuffleIndicesFile.c_str());
  }

  readShuffleIndices(shuffleIndices);
}

void ShuffleReindexer::readShuffleIndices(FILE* file) {
  long node = 0;
  long currentRow = 0;
  while (fscanf(file, "%*ld %ld\n", &node) != EOF) {
    shuffleHash[node] = currentRow;
    ++currentRow;
  }

  fclose(file);
}


void ShuffleReindexer::run() {
  char* line = new char[rowlen];
  long currentRow = 0;
  vector<long> edges;

  while (fgets(line, rowlen, inputGraph)) {
    if (strcmp(line, "\n") == 0) {
      fprintf(outputGraph, "%ld\n", shuffleHash[currentRow]);
      ++currentRow;
      continue;
    }

    if (strlen(line) > 0) {
      line[strlen(line) - 1]= ' ';
    }

    edges.clear();
    util.splitByToken(line, edges);

    fprintf(outputGraph, "%ld", shuffleHash[currentRow]);
    for (int i = 0; i < (int) edges.size(); ++i) {
      fprintf(outputGraph, " %ld", shuffleHash[edges[i]]);
    }
    fprintf(outputGraph, "\n");

    ++currentRow;
  }

  fclose(inputGraph);
  fclose(outputGraph);
}


