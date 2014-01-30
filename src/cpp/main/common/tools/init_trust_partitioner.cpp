#include <iostream>
#include <vector>
#include <string>
#include <limits.h>
#include <stdio.h>

using namespace std;

struct InputNodeNum {
  string name;
  long node_num;
  long mnode;
};

void ReadConfig(char* config_name, vector<InputNodeNum>* config) {
  FILE* f = fopen(config_name, "r");
  if (f == NULL) printf("Error opening file: %s.\n", config_name);
  int port;
  long nnode, nedge, mnode;
  char name[1024];
  while (!feof(f)) {
    fscanf(f, "%d %s %ld %ld %ld\n", &port, name, &nnode, &nedge, &mnode);
    InputNodeNum inode;
    inode.name = string(name);
    inode.node_num = nnode;
    inode.mnode = mnode;
    config->push_back(inode);
  }
  // END
  InputNodeNum inode;
  inode.name = string("END");
  inode.node_num = LONG_MAX;
  inode.mnode = LONG_MAX;
  config->push_back(inode);
  fclose(f);
}

int main (int argc, char* argv[]) {
  int part = 0;
  vector<InputNodeNum> config;
  ReadConfig(argv[1], &config);
  long key, act_max;
  FILE* full_init_trust_file = fopen(argv[2], "r");
  if (full_init_trust_file == NULL) printf("Error opening file: %s.\n", argv[2]);
  act_max = config[part+1].mnode;
  FILE* act_pr_part = fopen((config[part].name + string(".init_trust")).c_str(), "w");
  if (act_pr_part == NULL) printf("Error opening file: %s.\n", (config[part].name + string(".init_trust")).c_str());
  while (fscanf(full_init_trust_file, "%ld", &key ) != EOF) {
    if (key < act_max) {
      fprintf(act_pr_part, "%ld\n", key);   
    } else {
      fclose(act_pr_part);
      ++part;
      act_max = config[part+1].mnode;
      act_pr_part = fopen((config[part].name + string(".init_trust")).c_str(), "w");
      if (act_pr_part == NULL) printf("Error opening file: %s.\n", (config[part].name + string(".init_trust")).c_str());
      fprintf(act_pr_part, "%ld\n", key);   
    }
  }
  fclose(act_pr_part);
  fclose(full_init_trust_file);
}

/*int main (int argc, char* argv[]) {
  double pr;
  vector<InputNodeNum> config;
  ReadConfig(argv[1], &config);
  unsigned long key;
  FILE* full_init_trust_file = fopen(argv[2], "r");
  if (full_init_trust_file == NULL) printf("Error opening file: %s.\n", argv[2]);
  for (unsigned  i = 0; i < config.size(); ++i) {
    FILE* act_pr_part = fopen((config[i].name + string(".init_trust")).c_str(), "w");
    if (act_pr_part == NULL) printf("Error opening file: %s.init_trust.\n", config[i].name.c_str());
    //for (int j = 0; j < config[i].node_num; ++j) {
    while () {
      fscanf(full_init_trust_file, "%ld %lf\n", &key, &pr);
      fprintf(act_pr_part, "%lf\n", pr);
    }
    fclose(act_pr_part);
  }
  fclose(full_init_trust_file);
}*/
