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

class SimpleMockTestRun: public AlgoTestBase {
protected:

  SimpleMockTestRun() {
  }

  virtual ~SimpleMockTestRun() {
  }

  virtual void SetUp() {
    initLogger();
    initParams("SIMPLE_MOCK");
    vector<string> part1;
    vector<string> part2;

    //part1.push_back("");
    //part2.push_back("");


    addNodeFactory(part1, 2);
    addNodeFactory(part2, 2);
    /*addNodeFactory(4);
    addNodeFactory(4);*/

    setUpBuilder();
    finalSetup();
  }

  virtual void TearDown() {
  }

  void initParams(string nodeType) {
    AlgoTestBase::initParams(nodeType);
  }

  void addNodeFactory(vector<string> part, long numSlaves) {
    TestSimrankOddEvenNodeFactory* nodeFactory = new TestSimrankOddEvenNodeFactory();
    //vector<string> partString;
    AlgoTestBase::addNodeFactory(nodeFactory, part, numSlaves);
  }
};


TEST_F(SimpleMockTestRun, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
  cluster.init();
  //cluster.setUp();
  cluster.start();
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
