/*
 * filter_edge_list_builder.cpp
 *
 *  Created on: 2014.03.31.
 *      Author: kisstom
 */

#include <stdio.h>
#include "../../../../main/common/graph/filter_edge_list_builder.h"
#include "../../../../main/common/graph/edgelist_container.h"

int main (int argc, char* argv[]) {
  FilterEdgeListBuilder builder;
  EdgelistContainer container;
  long minnode = atol(argv[4]);

  container.initContainers();
  container.setMinnode(minnode);
  builder.readNodesToDelete(argv[1]);
  builder.setContainer(&container);
  builder.buildFromFile(argv[2]);

  FILE* f = fopen(argv[3], "w");
  container.flush(f);
  fclose(f);

  return 0;
}


