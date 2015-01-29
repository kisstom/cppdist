#include <gtest/gtest.h>

#include <gtest/gtest.h>
#include "../../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"
#include "../../../main/algos/algo_components/cluster.h"
#include "../new_algo_test_base.h"
#include "../../../main/common/graph/builder/test_edge_list_builder.h"
#include "../algo_test_util.h"

class SimrankOddEvenTest: public NewAlgoTestBase {
protected:

  SimrankOddEvenTest() {
    pathLen = 2;
    numPathes = 1;
  }

  virtual ~SimrankOddEvenTest() {
  }

  virtual void SetUp() {
    initLogger();
    addConfigParam("NODE_TYPE", "SIMRANK_ODD_EVEN");
    addConfigParam("INNER_MASTER_TYPE", "SIMRANK_ODD_EVEN");
    addConfigParam("DESERIALIZER_TYPE", "SIMRANK_ODD_EVEN");
    addConfigParam("MAX_ITER", "1");
    addConfigParam("PATH_LEN", util.intToString(2));
    addConfigParam("NUM_PATHES", util.intToString(1));
    addConfigParam("SEED", "13");
    addConfigParam("RANDOM_TYPE", "PSEUDO");

    TestSimrankOddEvenNodeFactory* nodeFactory = NULL;

    vector<vector<string> > partitions;
    vector<string> part1;
    vector<string> part2;

    part1.push_back("1 2 3");
    part1.push_back("5");
    part1.push_back("5");
    part1.push_back("");

    part2.push_back("");
    part2.push_back("4");
    part2.push_back("0 5");
    part2.push_back("1 4");

    partitions.push_back(part1);
    partitions.push_back(part2);

    vector<OldPartitionNodeFactory*> factories;
    long minnode = 0;
    for (int i = 0; i < (int) partitions.size(); ++i) {
      nodeFactory = new TestSimrankOddEvenNodeFactory;
      vector<list<long*> > fpStarts((int) partitions[i].size());

      algoTestUtil.setFingerPrintStarts(&fpStarts, partitions[i], pathLen, numPathes, minnode);

      nodeFactory->setFingerprints(fpStarts);
      factories.push_back(nodeFactory);
      minnode += (long) partitions[i].size();
    }

    algoTestUtil.createFactoriesFromPart(partitions, &factories);

    for (int i = 0; i < (int) factories.size(); ++i) {
      addNodeFactory(factories[i]);
    }
  }

  AlgoTestUtil algoTestUtil;
  int pathLen;
  int numPathes;
};


TEST_F(SimrankOddEvenTest, test) {
  initCluster();
  runCluster();
  SimrankOddEvenNode* node = static_cast<SimrankOddEvenNode*>(cluster->getNode(0));
  vector<list<long*> >* pathes = node->getPathes();
  for (int i = 0; i < (int) pathes->size(); ++i) {
    for (list<long*>::iterator it = (*pathes)[i].begin(); it != (*pathes)[i].end(); ++it) {
      int j = 0;
      long *foo = *it;
      while (*(foo + j) >= 0 && j < pathLen + 1) {
        printf("%ld ", *(foo + j));
        ++j;
      }
      printf("\n");
    }
  }

  vector<vector<long*> >* finishedPathes = node->getFinishedPathes();
  for (int i = 0; i < (int) finishedPathes->size(); ++i) {
    for (vector<long*>::iterator it = (*finishedPathes)[i].begin(); it != (*finishedPathes)[i].end(); ++it) {
      int j = 0;
      long *foo = *it;
      while (*(foo + j) >= 0 && j < pathLen + 1) {
        printf("%ld ", *(foo + j));
        ++j;
      }
      printf("\n");
    }
  }

  node = static_cast<SimrankOddEvenNode*>(cluster->getNode(1));
  pathes = node->getPathes();
  for (int i = 0; i < (int) pathes->size(); ++i) {
    for (list<long*>::iterator it = (*pathes)[i].begin(); it != (*pathes)[i].end(); ++it) {
      int j = 0;
      long *foo = *it;
      while (*(foo + j) >= 0 && j < pathLen + 1) {
        printf("%ld ", *(foo + j));
        ++j;
      }
      printf("\n");
    }
  }

  finishedPathes = node->getFinishedPathes();
  for (int i = 0; i < (int) finishedPathes->size(); ++i) {
    for (vector<long*>::iterator it = (*finishedPathes)[i].begin(); it != (*finishedPathes)[i].end(); ++it) {
      int j = 0;
      long *foo = *it;
      while (*(foo + j) >= 0 && j < pathLen + 1) {
        printf("%ld ", *(foo + j));
        ++j;
      }
      printf("\n");
    }
  }

  deleteCluster();
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
