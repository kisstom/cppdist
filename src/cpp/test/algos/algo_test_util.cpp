#include "algo_test_util.h"
#include "../../main/common/graph/builder/test_edge_list_builder.h"

void AlgoTestUtil::createFactoriesFromPart(vector<vector<string> > partitions,
     vector<OldPartitionNodeFactory*>* factories) {
  EdgelistContainer* cont = NULL;
  GraphPartitionConfigHandler* handler = NULL;
  vector<long>* partMinNodes = new vector<long>();
  vector<long>* partNumNodes = new vector<long>();

  TestEdgeListBuilder builder;

  for (int partI = 0; partI < (int) partitions.size(); ++partI) {
    long minnode = -1;
    if (partI == 0) {
      minnode = 0;
    } else {
      minnode = (long) partitions[partI - 1].size();
    }

    partMinNodes->push_back(minnode);
    partNumNodes->push_back((long) partitions[partI].size());
    handler = new GraphPartitionConfigHandler;
    cont = builder.buildFromString(partitions[partI], minnode);

    handler->setPartitionMinNodes(partMinNodes);
    handler->setPartitionNumNodes(partNumNodes);

    (*factories)[partI]->setEdgelistContainer(cont);
    (*factories)[partI]->setPartConfHandler(handler);
  }
}
