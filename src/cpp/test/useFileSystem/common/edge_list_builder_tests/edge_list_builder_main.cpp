/*
 * edge_list_builder_test.cpp
 *
 *  Created on: 2014.03.31.
 *      Author: kisstom
 */

#include <stdio.h>
#include "../../../../main/common/graph/edge_list_builder.h"
#include "../../../../main/common/graph/edgelist_container.h"

int main (int argc, char* argv[]) {
  EdgeListBuilder builder;
  EdgelistContainer container;
  long minnode = atol(argv[3]);

  container.initContainers();
  container.setMinnode(minnode);
  builder.setContainer(&container);
  builder.buildFromFile(argv[1]);

  FILE* f = fopen(argv[2], "w");
  container.flush(f);
  fclose(f);

  return 0;
}

