#include "print_clue_format.h"
#include <string.h>
#include <stdio.h>


void PrintClueFormat::ReadAndPrint(char* input, char* output, long numnodes) {
  FILE * in = fopen(input, "r");
  if (in == NULL) {
    fprintf(stderr, "Error opening file %s\n", input);
    return;
  }

  FILE * out = fopen(output, "w");
  if (out == NULL) {
    fprintf(stderr, "Error opening file %s\n", output);
    return;
  }

  char* line = new char[MAX_ROW_LEN];
  size_t pos;
  long last = -1, current;
  while (fgets(line, MAX_ROW_LEN, in) != NULL) {
    string str_line = string(line);
    pos = str_line.find_first_of(" ", 0);
    sscanf(str_line.substr(0, pos).c_str(), "%ld", &current);
    if (current - 1 != last) {
      for (long x = 0; x < current - 1 - last; ++x) {
         fprintf(out, "\n");       
      }
    }
    last = current;
    if (pos == string::npos) {
      fprintf(out, "\n");   
    } else {
      fprintf(out, "%s", str_line.substr(pos + 1).c_str());
    }
  }

  printf("%ld rows expected, already %ld\n", numnodes, last);

  if (numnodes - 1 > last) {
    for (long x = 0; x < numnodes - 1 - last; ++x) {
      fprintf(out, "\n");       
    }
  }
  fclose(in);
  fclose(out);
}
