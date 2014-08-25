#include <iostream>
#include "simrank.h"

void usage() {
  printf("Number of pages|length of path|number of pathes|dump?|fingerprints|needed pairs|output\n");
}


int main (int argc, char * argv[]) {
  if (argv[1] == NULL) {
    usage();
    exit(0);
  }

  SimRank sr(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atof(argv[4]));
  fstream in(argv[5], fstream::in);
  sr.read(in);
  fstream needSR(argv[6], fstream::in);

  sr.needSR(needSR);
  sr.setOutputStream(argv[7]);
  sr.computeNeedScore();

  needSR.close();
  in.close();
}

