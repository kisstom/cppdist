#include "gtest/gtest.h"
#include "../../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"
#include "../../../main/algos/algo_components/cluster.h"
#include "../new_algo_test_base.h"

class SimpleMockMultiTestSetup: public NewAlgoTestBase {
protected:

  SimpleMockMultiTestSetup() {
  }

  virtual ~SimpleMockMultiTestSetup() {
  }

  virtual void SetUp() {
    addConfigParam("NODE_TYPE", "SIMPLE_MOCK");
    addConfigParam("INNER_MASTER_TYPE", "SIMPLE_MOCK");
    addConfigParam("DESERIALIZER_TYPE", "SIMPLE_MOCK");
    addConfigParam("MULTI", "MULTICAST");

    INodeFactory* nodeFactory1 = new TestSimrankOddEvenNodeFactory();
    addNodeFactory(nodeFactory1);

    INodeFactory* nodeFactory2 = new TestSimrankOddEvenNodeFactory();
    addNodeFactory(nodeFactory2);
  }
};

TEST_F(SimpleMockMultiTestSetup, testSetup) {
  initCluster();
  setupCluster();

  AlgoBase* algo = cluster->getAlgo(0);
  algo = cluster->getAlgo(1);
  deleteCluster();
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
