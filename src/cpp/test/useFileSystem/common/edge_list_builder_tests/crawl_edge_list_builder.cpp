/*
 * crawl_edge_list_builder.cpp
 *
 *  Created on: 2014.03.31.
 *      Author: kisstom
 */

#include <stdio.h>
#include "../../../../main/common/graph/crawl_edge_list_builder.h"
#include "../../../../main/common/graph/edgelist_container.h"

int main (int argc, char* argv[]) {
  CrawlEdgeListBuilder builder(atol(argv[1]));
  EdgelistContainer container;

  builder.setContainer(&container);
  builder.buildFromFile(argv[2]);

  FILE* f = fopen(argv[3], "w");
  container.flush(f);
  fclose(f);

  return 0;
}


