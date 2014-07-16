/*
 * fpTreeLeaves.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include "fpTreeLeaves.h"
#include "../common/util/util.h"

FpTreeLeaves::FpTreeLeaves() {
}

FpTreeLeaves::~FpTreeLeaves() {
  //delete tmpStream;
}

void FpTreeLeaves::run(FILE* f) {
  char line [1024];

  while (fgets(line, 1024, f) != NULL) {
    line[strlen(line)-1] = '\0';
    addInnerNodes(line);
  }
}


void FpTreeLeaves::addInnerNodes(char* line) {
  std::vector<long> edges;
  Util::split(line, edges);
  long root = edges[edges.size() - 1];
  for (int i = 1; i < (int) edges.size() - 1; ++i) {
    nodes[root].insert(edges[i]);
  }
}

