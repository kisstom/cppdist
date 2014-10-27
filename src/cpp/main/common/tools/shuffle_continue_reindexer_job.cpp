#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../graph_converter/shuffle_continue_reindexer.h"

using std::string;

int main (int argc, char* argv[]) {
  if (argc != 6) {
    fprintf(stderr, "ERROR! Usage: input output newhash numnodes numparts\n");
    exit(1);
  }

  string inputF(argv[1]);
  string outputF(argv[2]);
  string hashF(argv[3]);
  long numNodes;
  int numParts;

  sscanf(argv[4], "%ld", &numNodes);
  sscanf(argv[5], "%d", &numParts);

  ShuffleContinueReindexer reindexer(inputF, outputF, hashF, numNodes, numParts);
  reindexer.init();
  reindexer.run();
}
