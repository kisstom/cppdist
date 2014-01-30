#include <iostream>
#include <stdlib.h>
#include "inverse.h"

/*
arg1: input graf
arg2: output indexing
 */

int main (int argc, char* argv[]) {
  Inverse inverse(argv[1], atol(argv[2]));
  inverse.run();
  inverse.writeToFile(argv[3]);
}
