#include <iostream>
#include <stdio.h>
#include "host_graph_creator.h"

int main(int argc, char* argv []) {
  if (argc != 5) {
    printf("Wrong number of parameters. Proper usage: host_to_node_file host_to_id_outfile graph_file output_graph\n");
    return 0;
  }
  HostGraphCreator creator;
  creator.init(argv[1], argv[2], argv[3], argv[4]);
  creator.run();
}
