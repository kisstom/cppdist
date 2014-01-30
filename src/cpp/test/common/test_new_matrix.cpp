#include <iostream>
#include "../../main/common/graph/vector_node_matrix.h"
//#include "../../main/algos/fingerprint/node.h"

/*
 input
 numnode
 numedge
 first node
 */
int main (int argc, char* argv[]) {

  VectorNodeMatrix cf;
  char master[] = "master";
  char out[] = "out";
  char log[] = "log";
  char trust[] = "trust";

  //Node node(master, -1, 0, 0, out, log, 0, 4, 0, trust); 

  //node.setTest();
  cf.init(argv[1], stderr, atol(argv[2]), atol(argv[3]), atol(argv[4]));
/*  vector<long>* edge_starts = cf.GetEdgeStart();
  vector<long>* edge_list = cf.GetEdgeList();
  node.SetStartEdges(edge_starts);
  node.SetEdgeList(edge_list);
  printf("start %ld end %ld\n", (*edge_starts)[730], (*edge_starts)[731]);
  for (int i = (*edge_starts)[730]; i < (*edge_starts)[731]; i++) {
    printf("%ld ", (*edge_list)[i]);
  }

  printf("rand out link %ld\n", node.RandOutLink(730));*/
  cf.printM();
  return 0;
}
