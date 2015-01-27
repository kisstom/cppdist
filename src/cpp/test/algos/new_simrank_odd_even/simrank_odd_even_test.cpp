#include <gtest/gtest.h>

#include <gtest/gtest.h>
#include "../../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"
#include "../../../main/algos/algo_components/cluster.h"
#include "../new_algo_test_base.h"
#include "../../../main/common/graph/builder/test_edge_list_builder.h"

//namespace {

class SimrankOddEvenTest: public NewAlgoTestBase {
protected:

  SimrankOddEvenTest() {
  }

  virtual ~SimrankOddEvenTest() {
  }

  virtual void SetUp() {
    initLogger();
    int pathLen = 2;
    int numPathes = 1;
    addConfigParam("NODE_TYPE", "SIMRANK_ODD_EVEN");
    addConfigParam("INNER_MASTER_TYPE", "SIMRANK_ODD_EVEN");
    addConfigParam("DESERIALIZER_TYPE", "SIMRANK_ODD_EVEN");
    addConfigParam("MAX_ITER", "1");
    addConfigParam("PATH_LEN", util.intToString(2));
    addConfigParam("NUM_PATHES", util.intToString(1));
    addConfigParam("SEED", "13");
    addConfigParam("RANDOM_TYPE", "PSEUDO");

    TestSimrankOddEvenNodeFactory* nodeFactory1 =
            new TestSimrankOddEvenNodeFactory;
    TestEdgeListBuilder builder;

    vector<string> part1;
    vector<string> part2;

    part1.push_back("1 2 3");
    part1.push_back("5");
    part1.push_back("5");
    part1.push_back("");
    EdgelistContainer* cont1 = builder.buildFromString(part1, 0);
    nodeFactory1->setEdgelistContainer(cont1);

    vector<list<long*> > fpStarts1((int) part1.size());
    for (int start = 0; start < (int) part1.size(); ++start) {
      for (int pathI = 0; pathI < numPathes; ++pathI) {
        long* path = new long[pathLen + 1];
        std::fill_n(path, pathLen + 1, -1);
        path[0] = start + 0;
        path[1] = start + 0;
        fpStarts1[pathI].push_back(path);
      }
    }

    vector<long>* partMinNodes = new vector<long>();
    vector<long>* partNumNodes = new vector<long>();
    partMinNodes->push_back(0);
    partNumNodes->push_back((int) part1.size());

    GraphPartitionConfigHandler* handler1 = new GraphPartitionConfigHandler;

    part2.push_back("");
    part2.push_back("4");
    part2.push_back("0 5");
    part2.push_back("1 4");
    EdgelistContainer* cont2 = builder.buildFromString(part2, (int) part1.size());

    vector<list<long*> > fpStarts2((int) part2.size());
    for (int start = 0; start < (int) part2.size(); ++start) {
      for (int pathI = 0; pathI < numPathes; ++pathI) {
        long* path = new long[pathLen + 1];
        std::fill_n(path, pathLen + 1, -1);
        path[0] = start + (int) part1.size();
        path[1] = start + (int) part1.size();
        fpStarts2[pathI].push_back(path);
      }
    }

    partMinNodes->push_back((int) part1.size());
    partNumNodes->push_back((int) part2.size());

    handler1->setPartitionMinNodes(partMinNodes);
    handler1->setPartitionNumNodes(partNumNodes);

    nodeFactory1->setPartConfHandler(handler1);
    nodeFactory1->setFingerprints(fpStarts1);
    addNodeFactory(nodeFactory1);

    GraphPartitionConfigHandler* handler2 = new GraphPartitionConfigHandler;
    handler2->setPartitionMinNodes(partMinNodes);
    handler2->setPartitionNumNodes(partNumNodes);

    TestSimrankOddEvenNodeFactory* nodeFactory2 =
                new TestSimrankOddEvenNodeFactory;
    nodeFactory2->setEdgelistContainer(cont2);
    nodeFactory2->setPartConfHandler(handler2);
    nodeFactory2->setFingerprints(fpStarts2);

    addNodeFactory(nodeFactory2);
  }
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
      while (*(foo + j) >= 0 && j < 2 + 1) {
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
      while (*(foo + j) >= 0 && j < 2 + 1) {
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
      while (*(foo + j) >= 0 && j < 2 + 1) {
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
      while (*(foo + j) >= 0 && j < 2 + 1) {
        printf("%ld ", *(foo + j));
        ++j;
      }
      printf("\n");
    }
  }

  deleteCluster();
}

//}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
