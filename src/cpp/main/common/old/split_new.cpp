#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <gflags/gflags.h>
#include <string>

DEFINE_int64(size, 0, "");
DEFINE_int64(numnode, 0, "");
DEFINE_int64(numedge, 0, "");
DEFINE_int32(init_slave_port, 6001, "");
DEFINE_string(remote, "", "");
DEFINE_string(input, "", "");
DEFINE_string(slavery_cfg, "", "");


using namespace std;

class makePart {
  public:
    void Init(string fname);
    void process();
  private:
    void print_next_part();
    FILE* input;
};


void makePart::Init(string fname) {
  if (fname.compare("") == 0) {
    input = stdin;
    return;
  }
  input = fopen(fname.c_str(), "r");
  if (input == NULL) {
    fprintf(stderr, "Error opening input file: %s\n", fname.c_str());
    return ;
  }

}

void makePart::process() {
  long entire_num_edge = 0;
  char line[100000];
  char partition_name[1000];
  int partition = 0;
  int slave_port = FLAGS_init_slave_port;
  sprintf(partition_name, "%s_%d.txt", FLAGS_remote.c_str(), partition);
  FILE* current_output = fopen(partition_name, "w");
  if (current_output == NULL) {
    fprintf(stderr, "Error opening output file: %s\n", partition_name);
    return ;
  } 
  FILE* slavery_cfg = fopen(FLAGS_slavery_cfg.c_str(), "w");
  if (slavery_cfg == NULL) {
    fprintf(stderr, "Error opening slavery file: %s\n", FLAGS_slavery_cfg.c_str());
    return ;
  }
  unsigned long first = 0, numedge = 0, numnode;
  string node;
  while  (fgets(line, 100000, input)) {
    line[strlen(line)-2] = '\0';
    string str_line = string(line);
    size_t last_pos = string::npos;
    size_t pos = str_line.find_first_of(" ");
    node = str_line.substr(0, pos);
    u_long u_node = atol(node.c_str());
    while (pos != string::npos) {
      last_pos = pos;
      pos = str_line.find_first_of(" ", pos + 1);
      ++numedge;
      ++entire_num_edge;
    }
    fprintf(current_output, "%s\n", str_line.c_str());
    numnode = u_node - first + 1;
    if (numedge >= FLAGS_size) {
      // Ha mar valojaban nincs is tobb el
      // a listaban csak, 0 kielu csucsok, akkor ahhoz
      // nem keszitunk egy uj particiot.
      if (entire_num_edge == FLAGS_numedge) break;
      // Write params to config file.
      fprintf(slavery_cfg, "%d %s_%d.txt %ld %ld %ld\n", 
          slave_port, FLAGS_remote.c_str(), partition, numnode, numedge, first);
      fclose(current_output);
      // Init next iteration.
      numedge = 0;
      first = u_node + 1;
      ++partition;
      ++slave_port;
      sprintf(partition_name, "%s_%d.txt", FLAGS_remote.c_str(), partition);
      current_output = fopen(partition_name, "w");
      if (current_output == NULL) {
        fprintf(stderr, "Error opening output file: %s\n", partition_name);
        return ;
      }
    }
  }
  while  (fgets(line, 100000, input)) {
    line[strlen(line)-2] = '\0';
    fprintf(current_output, "%s\n", line);  
  }
  numnode = FLAGS_numnode - first;
  fprintf(slavery_cfg, "%d %s_%d.txt %ld %ld %ld\n", 
        slave_port, FLAGS_remote.c_str(), partition, numnode, numedge, first); 
/*  numnode = FLAGS_numnode - first;
  if (numnode > 0) {
    fprintf(slavery_cfg, "%d %s_%d.txt %ld %ld %ld\n", 
      slave_port, FLAGS_remote.c_str(), partition, numnode, numedge, first);
  }*/
  fclose(current_output);
  fclose(input);
  fclose(slavery_cfg);
}

int main (int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  makePart mp;
  mp.Init(FLAGS_input);
  mp.process();
}
