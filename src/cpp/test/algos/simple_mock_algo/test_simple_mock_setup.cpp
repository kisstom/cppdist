/*
 * test_simple_mock2.cpp
 *
 *  Created on: 2014.06.16.
 *      Author: kisstom
 */


#include <gtest/gtest.h>
#include "../../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"
#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"

//namespace {

class SimpleMockTestSetup: public AlgoTestBase {
protected:


  SimpleMockTestSetup() {
  }

  virtual ~SimpleMockTestSetup() {
  }

  virtual void SetUp() {
    initParams("SIMPLE_MOCK");
    initLogger();

    addFactory(2);
    addFactory(2);
    //addFactory(3);

    setUpBuilder();
    finalSetup();
  }

  void addFactory(int numSlaves) {
    TestSimrankOddEvenNodeFactory* nodeFactory = new TestSimrankOddEvenNodeFactory();
    vector<string> partString;
    AlgoTestBase::addNodeFactory(nodeFactory, partString, numSlaves);
  }

};

TEST_F(SimpleMockTestSetup, testSetup) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
  cluster.init();
  cluster.setUp();

  Algo* algo = cluster.getAlgo(0);
  ASSERT_TRUE(NULL != (algo->socketManager_->getSenderSockets())[1]);
  ASSERT_TRUE(NULL == (algo->socketManager_->getSenderSockets())[0]);

  algo = cluster.getAlgo(1);
  ASSERT_TRUE(NULL != (algo->socketManager_->getSenderSockets())[0]);
  ASSERT_TRUE(NULL == (algo->socketManager_->getSenderSockets())[1]);
}

//}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
