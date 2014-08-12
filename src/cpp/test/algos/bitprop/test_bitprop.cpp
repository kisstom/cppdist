/*
 * test_bitprop.cpp
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

//#include <gtest/gtest.h>

#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"
#include "../../../main/algos/algo_components/test_bitprop_node_factory.h"

namespace {

class BitpropTest :public AlgoTestBase {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  BitpropTest() {
  }

  virtual ~BitpropTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
     initParams("BITPROP");
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

     addNodeFactory(part1, 2);
     addNodeFactory(part2, 2);

     setUpBuilder();
     finalSetup();
   }

  void initParams(string nodeType) {
    AlgoTestBase::initParams(nodeType);

    params_["NEIGHBORHOOD_SIZE"] = "4";
    params_["NUM_CODING_BYTES"] = "8";
    params_["EPSILON"] = "0.05";
    params_["EST_INDEX"] = "0";
  }

  void addNodeFactory(vector<string> partString, long numSlaves) {
    EdgelistContainer* container = createContainer(partString, numSlaves);
    TestBitpropNodeFactory* nodeFactory = new TestBitpropNodeFactory;

    nodeFactory->setContainer(container);
    std::vector<FailedEstimate>* failedEstimatedNodes = new std::vector<FailedEstimate>();
    failedEstimatedNodes->push_back(FailedEstimate(0.5, 0, 0));

    nodeFactory->setFailedEstimateNodes(failedEstimatedNodes);
    nodeFactory->setEstimatonHandler(new IEstimationHandler);

    AlgoTestBase::addNodeFactory(nodeFactory, partString, numSlaves);
  }

};


TEST_F(BitpropTest, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
  cluster.init();
  cluster.start();
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


