/*
 * inverse_partitition_maker_job.cpp
 *
 *  Created on: Sep 8, 2014
 *      Author: kisstom
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../graph/inverse_partition_maker.h"

using std::string;


int main (int argc, char* argv[]) {
  if (argc != 6) {
    fprintf(stderr, "ERROR! Usage: input outputDirPrefix slaveryFile numslaves rowlen\n");
    exit(1);
  }

  string inputF(argv[1]);
  string dirPrefix(argv[2]);
  string slaveryFile(argv[3]);
  int numslaves, rowlen;

  sscanf(argv[4], "%d", &numslaves);
  sscanf(argv[5], "%d", &rowlen);

  InversePartitionMaker maker(inputF, dirPrefix, slaveryFile, numslaves, rowlen);
  maker.run();
}


