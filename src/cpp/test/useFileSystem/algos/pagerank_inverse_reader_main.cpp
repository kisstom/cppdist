/*
 * pagerank_inverse_reader_main.cpp
 *
 *  Created on: 2014.09.10.
 *      Author: kisstom
 */

#include <iostream>
#include "../../../main/algos/clever_pagerank/clever_pagerank_node.h"

int main (int argc, char* argv[]) {
  CleverPagerankNode node(0, 0, 0, 0);
  node.readInverseNodeBounds(string(argv[1]));
  node.readInverseOutEdges(string(argv[2]));
}

