#include <iostream>
#include "reindexer.h"

/*
arg1: input graf
arg2: index mapper
arg3: converted graph
 */

int main (int argc, char* argv[]) {
  Reindexer reindexer;
  reindexer.readIndices(argv[2]);
  reindexer.readOldPrintNew(argv[1], argv[3]);
}
