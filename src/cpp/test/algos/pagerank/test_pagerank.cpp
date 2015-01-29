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
