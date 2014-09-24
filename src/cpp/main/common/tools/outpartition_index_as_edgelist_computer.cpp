/*
 * outpartition_index_as_edgelist_computer.cpp
 *
 *  Created on: 2014.09.24.
 *      Author: kisstom
 */

#include <iostream>
#include <cstdio>
#include "../io/outpartition_index_computer.h"
#include "../util/logger_factory.h"

int main (int argc, char* argv[]) {
  LoggerFactory::initLogger("INFO", "CONSOLE", "");
  log4cpp::Category* logger =
      &log4cpp::Category::getInstance(std::string("OutPartitionIndexAsEdgelistComputer"));

  string inputFile(argv[1]);
  string cfg(argv[2]);
  int numslaves, rowlen, partIndex;

  sscanf(argv[3], "%d", &numslaves);
  sscanf(argv[4], "%d", &rowlen);
  sscanf(argv[5], "%d", &partIndex);

  OutPartitionIndexComputer computer(inputFile, cfg, numslaves, rowlen, partIndex);
  computer.run();

  vector<set<int> >* outPartitions = computer.getOutPartitions();
  FILE* outPartFile = fopen(argv[6], "w");
  if (NULL == outPartFile) {
    logger->error("Error opening file %s.", argv[6]);
    return 1;
  }

  computer.flushAsEdgelistContainer(outPartFile);
  fclose(outPartFile);
}


