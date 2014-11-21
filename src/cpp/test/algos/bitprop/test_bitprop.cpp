/*
 * test_bitprop.cpp
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"
#include "../../../main/algos/algo_components/test_bitprop_node_factory.h"
#include "../../../main/algos/bitprop/thread_safe_memory_estimation_handler.h"

namespace {

class BitpropTest2 :public AlgoTestBase {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  BitpropTest2() {
  }

  virtual ~BitpropTest2() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
     estimationHandler = new ThreadSafeMemoryEstimationHandler;

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

     params_["NUM_CODING_BYTES"] = "1";
     int numCodingBytes = 1;
     unsigned char* rvb1 = new unsigned char[6 * numCodingBytes];
     unsigned char* rvb2 = new unsigned char[6 * numCodingBytes];

     rvb1[0] = 0;
     rvb1[1] = 1;
     rvb1[2] = 2;
     rvb1[3] = 3;
     rvb1[4] = 4;
     rvb1[5] = 5;

     rvb2[0] = 0;
     rvb2[1] = 1;
     rvb2[2] = 2;
     rvb2[3] = 3;
     rvb2[4] = 4;
     rvb2[5] = 5;

     addNodeFactory(part1, 2, rvb1);
     addNodeFactory(part2, 2, rvb2);

     setUpBuilder();
     finalSetup();
   }

  void initParams(string nodeType) {
    AlgoTestBase::initParams(nodeType);

    params_["NEIGHBORHOOD_SIZE"] = "3";
    params_["EPSILON"] = "0.05";
    params_["EST_INDEX"] = "0";
  }

  void addNodeFactory(vector<string> partString, long numSlaves, unsigned char* rvb) {
    TestBitpropNodeFactory* nodeFactory = new TestBitpropNodeFactory;

    EdgelistContainer* container = createContainer(partString, numSlaves);
    nodeFactory->setContainer(container);

    std::vector<FailedEstimate>* failedEstimatedNodes = new std::vector<FailedEstimate>();
    failedEstimatedNodes->push_back(FailedEstimate(0.5, 0, 0));

    nodeFactory->setFailedEstimateNodes(failedEstimatedNodes);
    nodeFactory->setEstimatonHandler(estimationHandler);
    nodeFactory->setRandomBits(rvb);

    AlgoTestBase::addNodeFactory(nodeFactory, partString, numSlaves);
  }

  ThreadSafeMemoryEstimationHandler* estimationHandler;
};


TEST_F(BitpropTest2, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_, &clusterNodeParams);
  cluster.init();
  cluster.start();

  ASSERT_NEAR(0.0, estimationHandler->acceptedEstimations[1][0], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[1][1], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[1][2], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[1][3], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[1][4], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[1][5], 0.001);

  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[1][6], 0.001);
  ASSERT_NEAR(2.603, estimationHandler->acceptedEstimations[1][7], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[1][8], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[1][9], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[1][10], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[1][11], 0.001);

  ASSERT_NEAR(0.0, estimationHandler->acceptedEstimations[2][0], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[2][1], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[2][2], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[2][3], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[2][4], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[2][5], 0.001);

  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[2][6], 0.001);
  ASSERT_NEAR(2.603, estimationHandler->acceptedEstimations[2][7], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[2][8], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[2][9], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[2][10], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[2][11], 0.001);

  ASSERT_NEAR(0.0, estimationHandler->acceptedEstimations[3][0], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[3][1], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[3][2], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[3][3], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[3][4], 0.001);
  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[3][5], 0.001);

  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[3][6], 0.001);
  ASSERT_NEAR(2.603, estimationHandler->acceptedEstimations[3][7], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[3][8], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[3][9], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[3][10], 0.001);
  ASSERT_NEAR(9.163, estimationHandler->acceptedEstimations[3][11], 0.001);
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


