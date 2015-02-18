/*
 * fpTreeLeaves.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include "fpTreeLeaves.h"
#include "../common/util/util.h"

FpTreeLeaves::FpTreeLeaves() {
  logger =  &log4cpp::Category::getInstance(std::string("FpTreeLeaves"));
}

FpTreeLeaves::~FpTreeLeaves() {
}

void FpTreeLeaves::run(FILE* f) {
  logger->info("Starting run.");
  fflush(stdout);
  int lineRead = 0;
  char line [1024];


  while (fgets(line, 1024, f) != NULL) {
    line[strlen(line)-1] = '\0';
    addInnerNodes(line);

    ++lineRead;
    if (lineRead % 10000000 == 0) {
      logger->info("Number of lines read %d", lineRead);
      fflush(stdout);
    }
  }
}

void FpTreeLeaves::addInnerNodes(char* line) {
  std::vector<long> edges;
  Util::split(line, edges);

  long root = edges[edges.size() - 1];
  short pathLen = (short) (edges.size() - 2);
  RootPath rootPath(root, pathLen);

  for (int i = 1; i < (int) edges.size() - 1; ++i) {
    nodes[rootPath].insert(edges[i]);
  }
}

