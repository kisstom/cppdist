/*
 * test_simple_mock2.cpp
 *
 *  Created on: 2014.06.16.
 *      Author: kisstom
 */


#include <gtest/gtest.h>

#include <gtest/gtest.h>
#include "../../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"
#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"

namespace {

class SimpleMockTestMultiRun: public AlgoTestBase {
protected:

  SimpleMockTestMultiRun() {
  }

  virtual ~SimpleMockTestMultiRun() {
  }

  virtual void SetUp() {
    initLogger();
    params_["MULTI"] = "1";
    initParams("SIMPLE_MOCK");

    addNodeFactory(2);
    addNodeFactory(2);

    setUpBuilder();
    finalSetup();
  }

  virtual void TearDown() {
  }

  void initParams(string nodeType) {
    AlgoTestBase::initParams(nodeType);
  }

  void addNodeFactory(long numSlaves) {
    TestSimrankOddEvenNodeFactory* nodeFactory = new TestSimrankOddEvenNodeFactory();
    vector<string> part;
    AlgoTestBase::addNodeFactory(nodeFactory, part, numSlaves);
  }
};


TEST_F(SimpleMockTestMultiRun, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_, &clusterNodeParams, &partitionMinNodes_);
  cluster.init();
  cluster.start();
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
