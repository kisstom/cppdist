/*
 * infectedTreeComputerJob.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include <stdio.h>

#include <stdlib.h>
#include <vector>
#include <iostream>
#include "../common/util/util.h"
#include "infectedTreeComputer.h"
#include "infectedNodeComputer.h"
#include "fpTreeLeaves.h"
#include "../common/util/logger_factory.h"

int main (int argc, char* argv[]) {
  LoggerFactory::initLogger("INFO", "CONSOLE", "");

  string inputGraphN(argv[1]);
  string fingerprintN(argv[2]);
  string outputN(argv[3]);
  int numCrawls = atoi(argv[4]);
  std::vector<long> crawlMaxes;

  for (int i = 0; i < numCrawls; ++i) {
    crawlMaxes.push_back(atol(argv[i + 5]));
  }

  FILE* inputGraph = fopen(inputGraphN.c_str(), "r");
  InfectedNodeComputer infectedNodeComputer(crawlMaxes);
  infectedNodeComputer.buildFromFile(inputGraph);
  std::cout << "Finished reading graph.\n";
  fflush(stdout);

  FILE* fingerprints = fopen(fingerprintN.c_str(), "r");
  FpTreeLeaves fpTreeNodes;
  fpTreeNodes.run(fingerprints);
  std::cout << "Finished reading fingerprints. FpTreeNodes size " << fpTreeNodes.nodes.size() << "\n";
  fflush(stdout);

  InfectedTreeComputer treeComputer(&fpTreeNodes.nodes, &infectedNodeComputer.infectedNodes);
  treeComputer.computeInfectedTrees();

  FILE* infectedTrees = fopen(outputN.c_str(), "w");
  treeComputer.flush(infectedTrees);

  fclose(inputGraph);
  fclose(fingerprints);
  fclose(infectedTrees);
}


