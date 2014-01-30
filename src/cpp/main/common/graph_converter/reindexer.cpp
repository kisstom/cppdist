#include "reindexer.h"
#include <string.h>
#include <stdexcept>
#include <stdio.h>

void Reindexer::readIndices(char* indices) {
  FILE* index_file = fopen(indices, "r");
  if (index_file == NULL) {
    fprintf(stderr, "Error opening file %s\n", indices);
    return;
  }

  long old_hash;
  long new_id;
  while (fscanf(index_file, "%ld %ld\n", &old_hash, &new_id) != EOF) {
    mapper_[old_hash] = new_id;
  }
  printf("%d keys stored.\n", (int) mapper_.size());
  fclose(index_file);
}

void Reindexer::readOldPrintNew (char* old_hash_f_name, char* reindex_f_name) {
  FILE* reindexed = fopen(reindex_f_name, "w");
  if (reindexed == NULL) {
    fprintf(stderr, "Error opening file %s\n", reindex_f_name);
    return;
  }

  FILE* old_hash_f = fopen(old_hash_f_name, "r");
  if (old_hash_f_name == NULL) {
    fprintf(stderr, "Error opening file %s\n", old_hash_f_name);
    return;
  }

  char* line = new char[MAX_ROW_LEN];
  size_t pos, epos;
  string str_node;
  long reindexed_node;
  int nr = 0;
  while (fgets(line, MAX_ROW_LEN, old_hash_f) != NULL) {
    string str_line = string(line);
    
    if (strlen(line) == 0) {
      // valami hiba uzi
      continue;
    }

    pos = str_line.find_first_of(" ", 0);
    str_node = str_line.substr(0, pos);
    reindexed_node = ReindexNode(str_node);
    fprintf(reindexed, "%ld", reindexed_node);

    while (pos != string::npos) {
      epos = str_line.find_first_of(" ", pos + 1);
      if (epos == string::npos) {
        str_node = str_line.substr(pos + 1);
      } else {
        str_node = str_line.substr(pos + 1, epos - pos);
      }
      reindexed_node = ReindexNode(str_node);
      fprintf(reindexed, " %ld", reindexed_node);
      pos = epos;
    }
    fprintf(reindexed, "\n");
    ++nr;
  }

  fclose(old_hash_f);
  fclose(reindexed);
  printf("%d rows processed.\n", nr);
  delete [] line;
}

long Reindexer::ReindexNode(string str_node) {
    long long_node, reindexed_node = 0;
    sscanf(str_node.c_str(), "%ld", &long_node);
    try {
      reindexed_node = mapper_[long_node];
    } catch (out_of_range& e) {
      fprintf(stderr, "Out of range error for hash node %ld\n", long_node);
    }
    return reindexed_node;
}
