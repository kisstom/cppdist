#include <iostream>
#include "continue_indexing.h"

/*
arg1: input graf
arg2: output indexing
 */

int main (int argc, char* argv[]) {
  Indexer indexer;
  indexer.init(argv[1]);
  indexer.run();
  indexer.flush(argv[2]);
}
