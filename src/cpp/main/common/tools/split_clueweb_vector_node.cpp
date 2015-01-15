#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class makePart {
  public:
    makePart(string _input, int _init_slave_port, int _numedge_perpart , string _part_prefix, string _partition_cfg) {
      FLAGS_input = _input;
      FLAGS_init_slave_port = _init_slave_port;
      FLAGS_numedge_perpart = _numedge_perpart;
      FLAGS_part_prefix = _part_prefix;
      FLAGS_partition_cfg = _partition_cfg;
      ROWLEN = 15000000;
    }
    void Init(string fname);
    void process();
    void curr_time();
  private:
    string FLAGS_input;
    int FLAGS_init_slave_port;
    int FLAGS_numedge_perpart;
    string FLAGS_part_prefix;
    string FLAGS_partition_cfg;
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
      fprintf(partition_cfg, "%ld %ld %ld\n", numnode, numedge, last_minnode);

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
  fprintf(partition_cfg, "%ld %ld %ld\n", numnode, numedge, last_minnode);

  fclose(current_output);
  fclose(partition_cfg);
  delete [] line;
}

int main (int argc, char* argv[]) {
  string FLAGS_input = string(argv[1]);
  int FLAGS_init_slave_port = atoi(argv[2]);
  int FLAGS_numedge_perpart = atoi(argv[3]);
  string FLAGS_part_prefix = string(argv[4]);
  string FLAGS_partition_cfg = string(argv[5]);

  cout << FLAGS_input << " " << FLAGS_init_slave_port << " " << FLAGS_numedge_perpart << " " << FLAGS_part_prefix << " " << FLAGS_partition_cfg << "\n";

  makePart mp(FLAGS_input, FLAGS_init_slave_port, FLAGS_numedge_perpart, FLAGS_part_prefix, FLAGS_partition_cfg);
  mp.Init(FLAGS_input);
  mp.process();
}
