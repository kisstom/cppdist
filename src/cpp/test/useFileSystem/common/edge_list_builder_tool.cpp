/*
 * test_edge_list_builder.cpp
 *
 *  Created on: 2014.03.10.
 *      Author: kisstom
 */


#include "../../../main/common/graph/edgelist_container.h"
#include "../../../main/common/graph/edge_list_builder.h"
#include "../../../main/common/util/logger_factory.h"

int main (int argc, char* argv[]) {
  LoggerFactory::initLogger("INFO", "CONSOLE", "");

  EdgelistContainer container;
  container.initContainers();
  container.setMinnode(0);

  EdgeListBuilder builder;

  builder.setContainer(&container);
  builder.buildFromFile(argv[1]);

  FILE* result = fopen(argv[2], "w");
  container.flush(result);
  fclose(result);

  return 0;
}

