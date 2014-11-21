/*
 * test_failed_est_use_prev.cpp
 *
 *  Created on: 2014.08.13.
 *      Author: kisstom
 */


#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"
#include "../../../main/algos/algo_components/test_bitprop_node_factory.h"
#include "../../../main/algos/bitprop/thread_safe_memory_estimation_handler.h"

namespace {

class BitpropTest4 :public AlgoTestBase {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  BitpropTest4() {
  }

  virtual ~BitpropTest4() {
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
     part1.push_back("0 2 3");
     part1.push_back("2 3");

     part2.push_back("1");
     part2.push_back("2");

     params_["NUM_CODING_BYTES"] = "1";
     int numCodingBytes = 1;
     unsigned char* rvb1 = new unsigned char[part1.size() * numCodingBytes];
     unsigned char* rvb2 = new unsigned char[part2.size() * numCodingBytes];

     rvb1[0] = 7;
     rvb1[1] = 56;

     rvb2[0] = 224;
     rvb2[1] = 0;

     std::vector<FailedEstimate>* failedEstimatedNodes1 = new std::vector<FailedEstimate>();
     std::vector<FailedEstimate>* failedEstimatedNodes2 = new std::vector<FailedEstimate>();
     failedEstimatedNodes2->push_back(FailedEstimate(0.5, 2, 1));
     failedEstimatedNodes2->push_back(FailedEstimate(0.5, 3, 1));

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


TEST_F(BitpropTest4, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_, &clusterNodeParams);
  cluster.init();
  cluster.start();

  ASSERT_NEAR(27.026, estimationHandler->failedEstimations[1][3], 0.001);
  ASSERT_NEAR(-1.0, estimationHandler->failedEstimations[1][2], 0.001);


}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

