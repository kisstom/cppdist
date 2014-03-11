/*
 * edge_list_builder_with_node.cpp
 *
 *  Created on: 2014.03.11.
 *      Author: kisstom
 */

#include "../../../../main/algos/simrank_odd_even/simrank_odd_even_node.h"
#include "../../../../main/common/graph/edge_list_builder.h"
#include "../../../../main/common/graph/edgelist_container.h"
#include "../../../../main/common/util/logger_factory.h"


int main (int argc, char* argv[]) {
  LoggerFactory::initLogger("INFO", "CONSOLE", "");

  char* inputFile = argv[1];
  char* outputFile = argv[2];
  long minnode = atol(argv[3]);
  long numnode = atol(argv[4]);
  long nextMinnode = atol(argv[5]);

  EdgeListBuilder builder;
  SimrankOddEvenNode node(-1, -1, -1, PSEUDO_RANDOM, numnode, minnode, nextMinnode);
  node.setEdgeListBuilder(&builder);
  node.initEdgeListContainer(string(inputFile));

  FILE* result = fopen(outputFile, "w");
  EdgelistContainer* container = builder.getContainer();
  container->flush(result);
  fclose(result);

  return 0;
}



