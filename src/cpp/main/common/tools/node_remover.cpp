/*
 * node_remover.cpp
 *
 *  Created on: 2014.03.10.
 *      Author: kisstom
 */

#include "../graph/filter_edge_list_builder.h"
#include "../util/logger_factory.h"

int main (int argc, char* argv[]) {
  LoggerFactory::initLogger("INFO", "CONSOLE", "");

  EdgelistContainer container;
  container.initContainers();
  container.setMinnode(0);

  FilterEdgeListBuilder builder;

  builder.setContainer(&container);
  builder.readNodesToDelete(argv[3]);
  builder.buildFromFile(argv[1]);

  FILE* result = fopen(argv[2], "w");
  container.flush(result);
  fclose(result);

  return 0;
}
