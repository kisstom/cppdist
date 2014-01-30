#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char* argv[]) {
  FILE* f = fopen(argv[1], "r");
  if (f == NULL) return 1;
  int row_max = atoi(argv[2]);

  bool fail = false;
  char* line = new char[row_max];
  long nr = 0;
  while (!feof(f)) {
    fgets(line, row_max, f);
    if (line[strlen(line) - 1] != '\n') {
      printf("%d chars not enough. %d is needed.\n", row_max, (int) strlen(line));
      fail = true;
      break;
    }
    ++nr;
    if (nr % 10000000 == 0) {
      printf("%ld rows processed.\n", nr);
    }
  }

  fclose(f);
  if (!fail) {
    printf("%d chars ok.\n", row_max);
  }
  delete [] line;
  return 0;
}
