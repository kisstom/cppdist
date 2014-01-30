#include <iostream>
#include "../util/ConfigParser.h"
#include "serialize.h"

/*
argv1: input
argv2: sered
 */

int main (int argc, char* argv[]) {
  SerializeResult sr;

  FILE* f = fopen(argv[2], "wb");
  if (f == NULL) {
    printf("Error opening file %s\n", argv[2]);
    return 0;
  }
  sr.Serialize(string(argv[1]), f);
  fclose(f);
  //sr.Deserialize(string(argv[2]), string(argv[3]));
}
