#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <tr1/unordered_map>

using namespace std;

int main (int argc, char *argv[]) {
  char name[100000];
  long id;
  time_t rawtime;
  tr1::unordered_map<long, string> trusted_pages;
  tr1::unordered_map<long, string> found_pages;
 
  FILE* trusted_pages_file = fopen(argv[1], "r");
  while (true) {
    fscanf(trusted_pages_file, "%ld", &id);
    if (feof(trusted_pages_file)) break;
    trusted_pages[id] = string("1");
  }
  fclose(trusted_pages_file);
  printf("Trusted pages are read.\n");
  fflush(stdout);

  long line = 0;
  while (fgets(name, 100000, stdin)) {
    if (trusted_pages.find(line) != trusted_pages.end()) {
      name[strlen(name)-1] = '\0';
      found_pages[line] = string(name);
    }
    ++line;
    if (line % 10000000 == 0) {
      time ( &rawtime );
      printf("%s%ld line processed.\n", ctime(&rawtime), line);
      fflush(stdout);
    }
  }
  printf("URL-s are read.");

  FILE* output = fopen(argv[2], "w");
  for (tr1::unordered_map<long, string>::unordered_map::iterator it = found_pages.begin();
          it != found_pages.end(); ++it) {
    fprintf(output, "%s %ld\n", it->second.c_str(), it->first);
  }
  fclose(output);
}
