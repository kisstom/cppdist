#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <gflags/gflags.h>
#include <string>

DEFINE_int64(numedge_perpart, 0, "");
DEFINE_int32(init_slave_port, 6001, "");
DEFINE_string(part_prefix, "", "");
DEFINE_string(input, "", "");
DEFINE_string(partition_cfg, "", "");


using namespace std;

class makePart {
  public:
    makePart() {
      ROWLEN = 15000000;
    }
    void Init(string fname);
    void process();
    void curr_time();
  private:
    FILE* input;
    int ROWLEN; 
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

void makePart::curr_time () {
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf ( "%s ", asctime (timeinfo) );
}

void makePart::process() {
  char* line = new char[ROWLEN];
  char partition_name[1000];
  int partition = 0;

  // Opens the first partition file.
  sprintf(partition_name, "%s_%d.txt", FLAGS_part_prefix.c_str(), partition);
  FILE* current_output = fopen(partition_name, "w");
  if (current_output == NULL) {
    fprintf(stderr, "Error opening output file: %s\n", partition_name);
    return ;
  }

  // Opens the partition cfg file.
  FILE* partition_cfg = fopen(FLAGS_partition_cfg.c_str(), "w");
  if (partition_cfg == NULL) {
    fprintf(stderr, "Error opening partition cfg file: %s\n", FLAGS_partition_cfg.c_str());
    return ;
  }

  long current_row = 0, lastNumEdge = 0;
  long minnode = current_row;
  long last_minnode = minnode;
  long numedge = 0;
  long numnode;
  size_t pos;
  int slave_port = FLAGS_init_slave_port;

  while (fgets(line, ROWLEN, input)) {
    if (current_row % 10000000 == 0) {
      curr_time();
      printf("%ld row processed.\n", current_row);
      fflush(stdout);
    }

    string str_line = string(line);
    if (str_line.compare("\n") == 0) {
      fprintf(current_output, "%s", line);
      ++current_row;
      continue;
    }

    if (numedge >= FLAGS_numedge_perpart) {
      fclose(current_output);

      // Current partition attributes.
      last_minnode = minnode;
      minnode = current_row;
      numnode = minnode - last_minnode;
      fprintf(partition_cfg, "%d %s_%d.txt %ld %ld %ld\n",
         slave_port, FLAGS_part_prefix.c_str(), partition, numnode, numedge, last_minnode);

      // Updates the attrs.
      numedge = 0;
      ++partition;
      ++slave_port;

      // Opens the next partition file.
      sprintf(partition_name, "%s_%d.txt", FLAGS_part_prefix.c_str(), partition);
      current_output = fopen(partition_name, "w");
      if (current_output == NULL) {
        fprintf(stderr, "Error opening output file: %s\n", partition_name);
        return ;
      }
    }

    if (str_line.size() > 0) {
      *(str_line.end() - 1) = ' ';
    }

    pos = str_line.find_first_of(" ", 0);
    while (pos != string::npos) {
      ++numedge;
      pos = str_line.find_first_of(" ", pos + 1);
    }
    fprintf(current_output, "%s", line);
    ++current_row;
    // Size of max numedge is exceeded.
  }

  // Last partition.
  last_minnode = minnode;
  minnode = current_row;
  numnode = minnode - last_minnode;
  fprintf(partition_cfg, "%d %s_%d.txt %ld %ld %ld\n",
      slave_port, FLAGS_part_prefix.c_str(), partition, numnode, numedge, last_minnode);

  fclose(current_output);
  fclose(partition_cfg);
  delete [] line;
}

int main (int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  makePart mp;
  mp.Init(FLAGS_input);
  mp.process();
}
