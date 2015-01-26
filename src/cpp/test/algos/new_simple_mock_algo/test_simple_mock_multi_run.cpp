#include <gtest/gtest.h>

#include <gtest/gtest.h>
#include "../../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"
#include "../../../main/algos/algo_components/cluster.h"
#include "../new_algo_test_base.h"

namespace {

class SimpleMockMultiTestRun: public NewAlgoTestBase {
protected:

  SimpleMockMultiTestRun() {
  }

  virtual ~SimpleMockMultiTestRun() {
  }

  virtual void SetUp() {
    addConfigParam("NODE_TYPE", "SIMPLE_MOCK");
    addConfigParam("INNER_MASTER_TYPE", "SIMPLE_MOCK");
    addConfigParam("DESERIALIZER_TYPE", "SIMPLE_MOCK");
    addConfigParam("MULTI", "1");

    INodeFactory* nodeFactory1 = new TestSimrankOddEvenNodeFactory();
    addNodeFactory(nodeFactory1);

    INodeFactory* nodeFactory2 = new TestSimrankOddEvenNodeFactory();
    addNodeFactory(nodeFactory2);
  }
};


TEST_F(SimpleMockMultiTestRun, test) {
  initCluster();
  //setupCluster();
  runCluster();

  AlgoBase* algo = cluster->getAlgo(0);
  algo = cluster->getAlgo(1);

  deleteCluster();
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
