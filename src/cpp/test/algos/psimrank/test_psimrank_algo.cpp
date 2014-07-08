/*
 * test_psimrank_algo.cpp
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#include <gtest/gtest.h>


#include "../../../main/algos/algo.h"
#include "../../../main/algos/master.h"
#include "../../../main/common/thread/node_thread.h"
#include "../../algos/simple_mock_algo/simple_mock_node.h"
#include "../../algos/simple_mock_algo/simple_mock_deserializer.h"
#include "../../algos/simple_mock_algo/simple_mock_inner_master.h"
#include "../../algos/simrank_odd_even/simrank_test_base.h"

namespace {

class PSimrankAlgoTest: public SimrankTestBase {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  PSimrankAlgoTest() {
  }

  virtual ~PSimrankAlgoTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
    initParams("PSIMRANK");
    initLogger();

    vector<string> part1;
    vector<string> part2;
    part1.push_back("1 2 3");
    part1.push_back("3");

    part2.push_back("3");
    part2.push_back("0 2");

    addPartition(part1, 2);
    addPartition(part2, 2);

    addExpectedPath(0, "0 1 3");
    addExpectedPath(0, "1 3 0");
    addExpectedPath(0, "2 3 0");
    addExpectedPath(0, "3 2 3");

    addExpectedPath(1, "0 3 2");
    addExpectedPath(1, "1 3 2");
    addExpectedPath(1, "2 3 2");
    addExpectedPath(1, "3 0 2");

    setUpBuilder();
    finalSetup();
  }

  void concat(Cluster& cluster) {
    concat_ = new vector<vector<long*> >;
    concat_->resize(numPathes_);

    PSimrankNode* node;
    vector<vector<long*> >* finished;
    vector<list<long*> >* pathes;

    for (int i = 0; i < slaveIndex_; ++i) {
      node = static_cast<PSimrankNode*>(cluster.getNode(i));
      finished = node->getFinishedPathes();
      for (int i = 0; i < (int) finished->size(); ++i) {

        for (vector<long*>::iterator it = (*finished)[i].begin();
            it != (*finished)[i].end(); ++it) {
          (*concat_)[i].push_back(*it);
        }

      }
      vector<list<long*> >* pathes = node->getPathes();
      for (int i = 0; i < (int) pathes->size(); ++i) {

        for (list<long*>::iterator it = (*pathes)[i].begin();
            it != (*pathes)[i].end(); ++it) {
          (*concat_)[i].push_back(*it);
        }

      }
    }

  }

  void initParams(string nodeType) {
    numPathes_ = 2;
    pathLen_ = 2;
    SimrankTestBase::initParams(nodeType);

    params_["RANDOM_TYPE"] = "PSEUDO";
    params_["RANDOM_A"] = "5,11,7,5";
    params_["RANDOM_B"] = "3,4,5,6";
  }

  virtual void TearDown() {
  }

  void addFactory() {
    nodeFactories_.push_back(new TestSimrankOddEvenNodeFactory);
  }
  // Objects declared here can be used by all tests in the test case for Foo.
};


TEST_F(PSimrankAlgoTest, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
  cluster.init();
  cluster.start();

  concat(cluster);
  check(concat_, &expectedPathes_);
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



