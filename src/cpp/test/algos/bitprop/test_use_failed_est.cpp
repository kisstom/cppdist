/*
 * test_use_failed_est.cpp
 *
 *  Created on: 2014.08.13.
 *      Author: kisstom
 */

#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"
#include "../../../main/algos/algo_components/test_bitprop_node_factory.h"
#include "../../../main/algos/bitprop/thread_safe_memory_estimation_handler.h"

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

     std::vector<FailedEstimate>* failedEstimatedNodes1 = new std::vector<FailedEstimate>();
     std::vector<FailedEstimate>* failedEstimatedNodes2 = new std::vector<FailedEstimate>();
     failedEstimatedNodes1->push_back(FailedEstimate(-1, 5, 1));
     failedEstimatedNodes2->push_back(FailedEstimate(0.5, 9, 1));
     failedEstimatedNodes2->push_back(FailedEstimate(0.5, 6, 2));

     addNodeFactory(part1, 2, rvb1, failedEstimatedNodes1);
     addNodeFactory(part2, 2, rvb2, failedEstimatedNodes2);

     setUpBuilder();
     finalSetup();
   }

  void initParams(string nodeType) {
    AlgoTestBase::initParams(nodeType);

    params_["NEIGHBORHOOD_SIZE"] = "2";
    params_["EPSILON"] = "0.05";
    params_["EST_INDEX"] = "1";
  }

  void addNodeFactory(vector<string> partString, long numSlaves, unsigned char* rvb,
      std::vector<FailedEstimate>* failedEstimatedNodes) {
    TestBitpropNodeFactory* nodeFactory = new TestBitpropNodeFactory;

    EdgelistContainer* container = createContainer(partString, numSlaves);
    nodeFactory->setContainer(container);
    nodeFactory->setFailedEstimateNodes(failedEstimatedNodes);
    nodeFactory->setEstimatonHandler(estimationHandler);
    nodeFactory->setRandomBits(rvb);

    AlgoTestBase::addNodeFactory(nodeFactory, partString, numSlaves);
  }

  ThreadSafeMemoryEstimationHandler* estimationHandler;
};


TEST_F(BitpropTest, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_, &clusterNodeParams, &partitionMinNodes_);
  cluster.init();
  cluster.start();

  ASSERT_NEAR(5.608, estimationHandler->acceptedEstimations[1][5], 0.001);
  ASSERT_NEAR(4.8315, estimationHandler->acceptedEstimations[2][6], 0.001);
  ASSERT_NEAR(3.054, estimationHandler->acceptedEstimations[1][9], 0.001);

}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}





