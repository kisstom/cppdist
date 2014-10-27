#include "shuffle_continue_reindexer.h"
#include <cstdio>

ShuffleContinueReindexer::ShuffleContinueReindexer(
    string inp, string outp, string reindex, long _numNodes, int _numParts) {
  inputGraphFile = inp;
  outputGraphFile = outp;
  newIndicesFile = reindex;
  numNodes = _numNodes;
  numParts = _numParts;
  nodeCounter = 0;
  numNodePerPart = numNodes / numParts;

  if (numNodes % numParts == 0) {
    samePartSize = true;
  } else {
    samePartSize = false;
  }
}

long ShuffleContinueReindexer::reindexer(long node) {
  if (newIndices.find(node) != newIndices.end()) {
    return newIndices[node];
  }

  if (samePartSize) {
    newIndices[node] = nodeCounter % numParts * numNodePerPart + nodeCounter / numParts;
  }  else {
    if (nodeCounter % numParts >= numNodes % numParts) {
      newIndices[node] = nodeCounter % numParts * numNodePerPart + numNodes % numParts + nodeCounter / numParts;
    } else {
      newIndices[node] = nodeCounter % numParts * (numNodePerPart + 1) + nodeCounter / numParts;
    }
  }

  fprintf(reindexFile, "%ld %ld %ld\n", nodeCounter, node, newIndices[node]);

  ++nodeCounter;
  return newIndices[node];
}

void ShuffleContinueReindexer::init() {
  inputGraph = fopen(inputGraphFile.c_str(), "r");
  outputGraph = fopen(outputGraphFile.c_str(), "w");
  reindexFile = fopen(newIndicesFile.c_str(), "w");

  if (NULL == inputGraph)  {
    fprintf(stderr, "Error opening file: %s.\n", inputGraphFile.c_str());
  }

  if (NULL == outputGraph)  {
    fprintf(stderr, "Error opening file: %s.\n", outputGraphFile.c_str());
  }

  if (NULL == reindexFile)  {
    fprintf(stderr, "Error opening file: %s.\n", newIndicesFile.c_str());
  }
}

void ShuffleContinueReindexer::run() {
  long from, to, fromReindex, toReindex, prevFrom = -1;
  bool inner = false;
  while (fscanf(inputGraph, "%ld\t%ld\n", &from, &to) != EOF) {
    fromReindex = reindexer(from);
    toReindex = reindexer(to);

    if (fromReindex != prevFrom) {
      prevFrom = fromReindex;
      if (inner) {
        fprintf(outputGraph, "\n");
      } else {
        inner = true;
      }

      fprintf(outputGraph, "%ld", fromReindex);
    }

    fprintf(outputGraph, " %ld", toReindex);
  }
  fprintf(outputGraph, "\n");

  fclose(inputGraph);
  fclose(outputGraph);
  fclose(reindexFile);
}
