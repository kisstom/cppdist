#include <iostream>
#include "print_clue_format.h"
#include <stdio.h>
/*
argv1: with key
argv2: ohne key
 */

int main (int argc, char* argv[]) {
  PrintClueFormat formatter;
  long numnodes;
  sscanf(argv[3], "%ld", &numnodes);
  formatter.ReadAndPrint(argv[1], argv[2], numnodes);
}
