/*
 * shuffle_reindexer_job.cpp
 *
 *  Created on: 2014.09.18.
 *      Author: kisstom
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../graph_converter/shuffle_reindexer.h"

using std::string;


int main (int argc, char* argv[]) {
  if (argc != 4) {
    fprintf(stderr, "ERROR! Usage: input output hash\n");
    exit(1);
  }

  string inputF(argv[1]);
  string outputF(argv[2]);
  string hashF(argv[3]);

  ShuffleReindexer reindexer(inputF, outputF, hashF);
  reindexer.init();
  reindexer.run();
}
