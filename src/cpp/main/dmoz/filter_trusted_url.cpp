#include <iostream>
#include <string>
#include <set>
#include <string.h>
#include <stdio.h>
#include <tr1/unordered_map>

using namespace std;

int main (int argc, char *argv[]) {
  char name[100000];
  time_t rawtime;
  set<string> trusted_urls;
 
  FILE* trusted_url_file = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");

  char url[100000];
  while (true) {
    fscanf(trusted_url_file, "%s", url);
    if (feof(trusted_url_file)) break;
    if (url[strlen(url) - 1] == '\n') url[strlen(url) - 1] = '\0';
    //printf("%s\n", url);
    trusted_urls.insert(string(url));
  }
  fclose(trusted_url_file);
  printf("Trusted urls are read.\n");
  fflush(stdout);

  long line = 0;
  while (fgets(name, 100000, stdin)) {
    if (name[strlen(name) - 1] == '\n') name[strlen(name) - 1] = '\0';
    if (trusted_urls.find(string(name)) != trusted_urls.end()) {
      fprintf(output, "%s %ld\n", name, line);
    }
    ++line;
    if (line % 10000000 == 0) {
      time ( &rawtime );
      printf("%s%ld line processed.\n", ctime(&rawtime), line);
      fflush(stdout);
    }
  }
  fclose(output);
}
