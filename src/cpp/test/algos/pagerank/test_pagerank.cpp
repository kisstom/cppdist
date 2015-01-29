#include <gtest/gtest.h>

#include <gtest/gtest.h>
#include "../../../main/algos/algo_components/test_node_factories/test_pagerank_node_factory.h"
#include "../../../main/algos/algo_components/cluster.h"
#include "../new_algo_test_base.h"
#include "../../../main/common/graph/builder/test_edge_list_builder.h"
#include "../algo_test_util.h"

//namespace {

class PagerankTest: public NewAlgoTestBase {
protected:

  PagerankTest() {
  }

  virtual ~PagerankTest() {
  }

  virtual void SetUp() {
    initLogger();
    addConfigParam("NODE_TYPE", "PAGERANK");
    addConfigParam("INNER_MASTER_TYPE", "PAGERANK");
    addConfigParam("DESERIALIZER_TYPE", "PAGERANK");
    addConfigParam("MAX_ITER", "1");
    addConfigParam("DUMP", "0.1");
    addConfigParam("NUMLINE", "8");

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
    for (int i = 0; i < (int) partitions.size(); ++i) {
      factories.push_back(new TestPagerankNodeFactory);
    }

    algoTestUtil.createFactoriesFromPart(partitions, &factories);

    for (int i = 0; i < (int) factories.size(); ++i) {
      addNodeFactory(factories[i]);
    }

  }

  /*virtual void SetUp() {
    initLogger();
    addConfigParam("NODE_TYPE", "PAGERANK");
    addConfigParam("INNER_MASTER_TYPE", "PAGERANK");
    addConfigParam("DESERIALIZER_TYPE", "PAGERANK");
    addConfigParam("MAX_ITER", "1");
    addConfigParam("DUMP", "0.1");
    addConfigParam("NUMLINE", "8");

    TestPagerankNodeFactory* nodeFactory1 = new TestPagerankNodeFactory;
    TestEdgeListBuilder builder;

    vector<string> part1;
    vector<string> part2;

    part1.push_back("1 2 3");
    part1.push_back("5");
    part1.push_back("5");
    part1.push_back("");
    EdgelistContainer* cont1 = builder.buildFromString(part1, 0);
    nodeFactory1->setEdgelistContainer(cont1);

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

    partMinNodes->push_back((int) part1.size());
    partNumNodes->push_back((int) part2.size());

    handler1->setPartitionMinNodes(partMinNodes);
    handler1->setPartitionNumNodes(partNumNodes);

    nodeFactory1->setPartConfHandler(handler1);
    addNodeFactory(nodeFactory1);

    GraphPartitionConfigHandler* handler2 = new GraphPartitionConfigHandler;
    handler2->setPartitionMinNodes(partMinNodes);
    handler2->setPartitionNumNodes(partNumNodes);

    TestPagerankNodeFactory* nodeFactory2 = new TestPagerankNodeFactory();
    nodeFactory2->setEdgelistContainer(cont2);
    nodeFactory2->setPartConfHandler(handler2);

    addNodeFactory(nodeFactory2);
  }*/

  AlgoTestUtil algoTestUtil;
};


TEST_F(PagerankTest, test) {
  initCluster();
  runCluster();

  AlgoBase* algo = cluster->getAlgo(0);
  algo = cluster->getAlgo(1);

  PagerankNode* node = static_cast<PagerankNode*>(cluster->getNode(0));
  vector<double>* prScore_ = node->getPagerankScore();

  ASSERT_NEAR(0.06875, prScore_->at(0), 0.0001);
  ASSERT_NEAR(0.10625, prScore_->at(1), 0.0001);
  ASSERT_NEAR(0.05, prScore_->at(2), 0.0001);
  ASSERT_NEAR(0.05, prScore_->at(3), 0.0001);

  node = static_cast<PagerankNode*>(cluster->getNode(1));
  prScore_ = node->getPagerankScore();
  ASSERT_NEAR(0.1812, prScore_->at(0), 0.0001);
  ASSERT_NEAR(0.29375, prScore_->at(1), 0.0001);
  ASSERT_NEAR(0.0125, prScore_->at(2), 0.0001);
  ASSERT_NEAR(0.0125, prScore_->at(3), 0.0001);

  deleteCluster();
}

//}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
