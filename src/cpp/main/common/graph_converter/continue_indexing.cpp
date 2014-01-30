#include "continue_indexing.h"
#include <string.h>
#include <stdio.h>

void Indexer::init(char* input) {
  input_ = fopen(input, "r");
  if (input_ == NULL) {
    fprintf(stderr, "Error opening file %s\n", input);
    return;
  }
  index_ = 0; 
}


void Indexer::run() {
  char* line = new char[MAX_ROW_LEN];
  size_t pos, epos;
  string str_node;
  int nr = 0;
  bool printNext = false;
  while (fgets(line, MAX_ROW_LEN, input_) != NULL) {
    string str_line = string(line);
    
    if (printNext) {
      printf("Nem a sorvegeig olvastuk. Ajjajjajj. %d. Jo hossz %d\n", nr, (int)strlen(line));
      printNext = false;
      fflush(stdout);
    } 

    if (line[strlen(line) - 1] != '\n') {
      // valami hiba uzi
      printf("Nem a sorvegeig olvastuk. Ajjajjajj. %d. Jo hossz %d\n", nr, (int)strlen(line));
      printNext = true;
      fflush(stdout);
    }

    pos = str_line.find_first_of(" ", 0);
    str_node = str_line.substr(0, pos);
    reindex(str_node);

    while (pos != string::npos) {
      epos = str_line.find_first_of(" ", pos + 1);
      if (epos == string::npos) {
        str_node = str_line.substr(pos + 1);
      } else {
        str_node = str_line.substr(pos + 1, epos - pos);
      }
      reindex(str_node);
      pos = epos;
    }

    ++nr;
  }
  fclose(input_);
  delete[] line;
  printf("%d rows processed.\n", nr);
}

void Indexer::flush(char* output) {
  FILE * out = fopen(output, "w");
  if (out == NULL) {
    fprintf(stderr, "Error opening file %s\n", output);
    return;
  }
  for (tr1::unordered_map<long, long>::iterator it = mapper_.begin(); it != mapper_.end(); ++it) {
    fprintf(out, "%ld %ld\n", it->first, it->second);
  }
  fclose(out);
}

void Indexer::reindex(string str_node) {
    long long_node;
    sscanf(str_node.c_str(), "%ld", &long_node);
    if (mapper_.find(long_node) == mapper_.end()) {
      mapper_[long_node] = index_++;
    }
}
