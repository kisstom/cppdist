#include <stdio.h>
#include <cstdlib>
#include "../../../../main/common/graph/builder/adjacency_list_builder.h"
#include "../../../../main/common/graph/adjacency_list.h"
#include "../../../../main/common/graph/entry.h"
#include "../../../../main/common/util/logger_factory.h"

int main (int argc, char* argv[]) {
  LoggerFactory::initLogger("INFO", "CONSOLE", "");

  AdjacencyListBuilder builder;

  AdjacencyList<Entry> container;
  long minnode = atol(argv[2]);

  container.initContainers();
  container.setMinnode(minnode);
  builder.buildFromFile(string(argv[1]), &container);

  FILE* f = fopen(argv[3], "w");
  container.flush(f);
  fclose(f);

  return 0;
}
