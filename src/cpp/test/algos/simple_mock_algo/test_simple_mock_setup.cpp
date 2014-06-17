/*
 * test_simple_mock2.cpp
 *
 *  Created on: 2014.06.16.
 *      Author: kisstom
 */


#include <gtest/gtest.h>


#include "../../../main/algos/algo.h"
#include "../../../main/algos/master.h"
#include "../../../main/common/thread/node_thread.h"
#include "../../algos/simple_mock_algo/simple_mock_node.h"
#include "../../algos/simple_mock_algo/simple_mock_deserializer.h"
#include "../../algos/simple_mock_algo/simple_mock_inner_master.h"
#include "../common/algo_test_base.h"

namespace {

class SimpleMockTestSetup: public AlgoTestBase {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  SimpleMockTestSetup() {
  }

  virtual ~SimpleMockTestSetup() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
    initParams("SIMPLE_MOCK");
    initLogger();

    vector<string> part1;
    vector<string> part2;
    part1.push_back("1 2 3");
    part1.push_back("");
    part1.push_back("9");
    part1.push_back("8 10");
    part1.push_back("");
    part1.push_back("2 4 6");

    part2.push_back("");
    part2.push_back("2 3 5");
    part2.push_back("11");
    part2.push_back("");
    part2.push_back("");
    part2.push_back("1 6 8");

    addPartition(part1, 2);
    addPartition(part2, 2);


    setUpBuilder();
    finalSetup();
  }

  virtual void TearDown() {
  }

  void addFactory() {
    nodeFactories_.push_back(new TestSimrankOddEvenNodeFactory);
  }
  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(SimpleMockTestSetup, testSetup) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
  cluster.init();
  cluster.setUp();
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
