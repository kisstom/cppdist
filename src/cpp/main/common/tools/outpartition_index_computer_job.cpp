/*
 * outpartition_index_computer_job.cpp
 *
 *  Created on: 2014.09.10.
 *      Author: kisstom
 */

#include <iostream>
#include <cstdio>
#include "../io/outpartition_index_computer.h"
#include "../util/logger_factory.h"

int main (int argc, char* argv[]) {
  LoggerFactory::initLogger("INFO", "CONSOLE", "");

  string inputFile(argv[1]);
  string cfg(argv[2]);
  int numslaves, rowlen, partIndex;

  sscanf(argv[3], "%d", &numslaves);
  sscanf(argv[4], "%d", &rowlen);
  sscanf(argv[5], "%d", &partIndex);

  OutPartitionIndexComputer computer(inputFile, cfg, numslaves, rowlen, partIndex);
  computer.run();

  vector<int>* nneighbors = computer.getNumNeighbors();
  FILE* neighborFile = fopen(argv[6], "w");
  for (int i = 0; i < (int) nneighbors->size(); ++i) {
    fprintf(neighborFile, "%d\n", (*nneighbors)[i]);
  }
  fclose(neighborFile);

  vector<set<int> >* outPartitions = computer.getOutPartitions();
  FILE* outPartFile = fopen(argv[7], "w");
  bool first = true;
  for (int i = 0; i < (int) outPartitions->size(); ++i) {
    first = true;
    for (set<int>::const_iterator it = (*outPartitions)[i].begin();
        it != (*outPartitions)[i].end(); ++it) {
      if (first) {
        fprintf(neighborFile, "%d", *it);
        first = false;
      } else {
        fprintf(neighborFile, " %d", *it);
      }
    }
    fprintf(neighborFile, "\n");
  }
  fclose(outPartFile);

}


