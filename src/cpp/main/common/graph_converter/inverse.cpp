#include "inverse.h"
#include <time.h>


Inverse::Inverse(char * input, int nnode) {
  nnode_ = nnode;
  input_ = fopen(input, "r");
  if (input_ == NULL) {
    fprintf(stderr, "Error opening file %s\n", input);
  }
  inverse_.resize(nnode);
  MAX_ROW_LEN = 15000000;
}

void Inverse::mytime() {
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf ( "%s ", asctime (timeinfo) );

}

void Inverse::writeToFile(char *output) {
  output_ = fopen(output, "w");
  if (output_ == NULL) {
    fprintf(stderr, "Error opening file %s\n", output);
    return;
  }
  for (int i = 0; i < (int)inverse_.size(); ++i) {
    if (inverse_[i].size() >= 1) {
      fprintf(output_, "%ld", inverse_[i][0]);
    }
    for (int j = 1; j < (int)inverse_[i].size(); ++j) {
      fprintf(output_, " %ld", inverse_[i][j]);
    }
    fprintf(output_, "\n");
  }
  fclose(output_);
}

void Inverse::run() {
  char* line = new char[MAX_ROW_LEN];
  size_t pos, epos;
  string str_node;
  long nr = 0;
  bool printNext = false;
  long dest;
  while (fgets(line, MAX_ROW_LEN, input_) != NULL) {
    string str_line = string(line);
    
    if (printNext) {
      printf("Nem a sorvegeig olvastuk. Ajjajjajj. %ld. Jo hossz %d\n", nr, (int)strlen(line));
      printNext = false;
      fflush(stdout);
    } 

    if (line[strlen(line) - 1] != '\n') {
      // valami hiba uzi
      printf("Nem a sorvegeig olvastuk. Ajjajjajj. %ld. Jo hossz %d\n", nr, (int)strlen(line));
      printNext = true;
      fflush(stdout);
    }

    if (strlen(line) != 1) {
      pos = string::npos;
      while (1 == 1) {
        epos = str_line.find_first_of(" ", pos + 1);
        if (epos == string::npos) {
          str_node = str_line.substr(pos + 1);
        } else {
          str_node = str_line.substr(pos + 1, epos - pos);
        }
        sscanf(str_node.c_str(), "%ld", &dest);
        if (dest >= nnode_) {
          printf("In %ld found %ld\n", nr, dest);
        }
        inverse_[dest].push_back(nr);
        pos = epos;
        if (pos == string::npos) break;
      }
    }
    ++nr;
    if (nr % 1000000 == 0) {
      mytime();
      printf("%ld row done.\n", nr);
      fflush(stdout);
    }
  }
  fclose(input_);

  delete[] line;
  printf("%ld rows processed.\n", nr);
}

