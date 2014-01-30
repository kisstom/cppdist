#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <tr1/unordered_map>

using namespace std;

int main (int argc, char *argv[]) {
  FILE* output = fopen(argv[1], "w");
  char name[1000000];
  time_t rawtime;
 
  long line = 0;
  while (fgets(name, 1000000, stdin)) {
    if (strlen(name) <= 1) {
      fprintf(output, "%ld\n", line);
    } else {
      fprintf(output, "%ld %s", line, name);
    }
    ++line;
    if (line % 10000000 == 0) {
      time ( &rawtime );
      printf("%s%ld line processed.\n", ctime(&rawtime), line);
      fflush(stdout);
    }
  }
  printf("URL-s are read.");
  fclose(output);
}
