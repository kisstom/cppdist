#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"
#include "../../../main/algos/algo_components/test_node_factories/test_counter_inverse_pagerank_node_factory.h"

class CounterInversePagerankNodeTest: public AlgoTestBase {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  CounterInversePagerankNodeTest() {
  }

  virtual ~CounterInversePagerankNodeTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
     initParams("COUNTER_INVERSE_PAGERANK");
     initLogger();

     params_["MAX_ITER"] = "1";
     params_["DUMP"] = "0.1";

     vector<string> part1;
     vector<string> part2;
     vector<long>* partitionBounds = new vector<long>(3);
     (*partitionBounds)[0] = 0;
     (*partitionBounds)[1] = 2;
     (*partitionBounds)[2] = 4;

     part1.push_back("");
     part1.push_back("0 3");
     part1.push_back("0 1 2");
     part1.push_back("0 2");
     part1.push_back("1");
     part1.push_back("2");
     part1.push_back("1 3");

     part2.push_back("");
     part2.push_back("1 3");
     part2.push_back("0");
     part2.push_back("1");
     part2.push_back("2");

     vector<string> partIndices1;
     vector<string> partIndices2;
     partIndices1.push_back("0");
     partIndices1.push_back("");
     partIndices1.push_back("1");
     partIndices1.push_back("1");
     partIndices1.push_back("");
     partIndices1.push_back("0");
     partIndices1.push_back("");

     partIndices2.push_back("0");
     partIndices2.push_back("1");
     partIndices2.push_back("");
     partIndices2.push_back("");
     partIndices2.push_back("0 1");

     vector<int>* numNeighbors1 = new vector<int>();
     vector<int>* numNeighbors2 = new vector<int>();
     numNeighbors1->push_back(3);
     numNeighbors1->push_back(0);
     numNeighbors1->push_back(1);
     numNeighbors1->push_back(2);
     numNeighbors1->push_back(0);
     numNeighbors1->push_back(3);
     numNeighbors1->push_back(0);

     numNeighbors2->push_back(3);
     numNeighbors2->push_back(1);
     numNeighbors2->push_back(0);
     numNeighbors2->push_back(0);
     numNeighbors2->push_back(3);

     addNodeFactory(part1, partIndices1, partitionBounds, numNeighbors1, 2);
     addNodeFactory(part2, partIndices2, partitionBounds, numNeighbors2, 2);

     setUpBuilder();
     finalSetup();
   }

  void initParams(string nodeType) {
    AlgoTestBase::initParams(nodeType);
  }

  void addNodeFactory(vector<string> pointerToCounters, vector<string> outPartitionIndices,
     vector<long>* partitionBound, vector<int>* numNeighbors, long numSlaves) {
    TestCounterInversePagerankNodeFactory* nodeFactory = new TestCounterInversePagerankNodeFactory;
    EdgelistContainer* outpContainer = createContainer(outPartitionIndices, numSlaves);
    EdgelistContainer* counterContainer = createContainer(pointerToCounters, numSlaves);

    nodeFactory->setOutPartitionIndices(outpContainer);
    nodeFactory->setPartitionBound(partitionBound);
    nodeFactory->setPointerToCounters(counterContainer);
    nodeFactory->setNumSlaves(numSlaves);
    nodeFactory->setNumNeighbors(numNeighbors);
    // TODO size is enough to pass
    AlgoTestBase::addNodeFactory(nodeFactory, pointerToCounters, numSlaves);
  }
};


TEST_F(CounterInversePagerankNodeTest, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_, &clusterNodeParams);
  cluster.init();
  cluster.start();

  CounterInversePagerankNode* node = static_cast<CounterInversePagerankNode*>(cluster.getNode(0));

  ASSERT_NEAR(0.008333, node->pagerankScore->at(0), 0.0001);
  ASSERT_NEAR(0.058333, node->pagerankScore->at(1), 0.0001);
  ASSERT_NEAR(0.08333, node->pagerankScore->at(2), 0.0001);
  ASSERT_NEAR(0.05833, node->pagerankScore->at(3), 0.0001);
  ASSERT_NEAR(0.03333, node->pagerankScore->at(4), 0.0001);
  ASSERT_NEAR(0.03333, node->pagerankScore->at(5), 0.0001);
  ASSERT_NEAR(0.05833, node->pagerankScore->at(6), 0.0001);

  node = static_cast<CounterInversePagerankNode*>(cluster.getNode(1));
  ASSERT_NEAR(0.008333, node->pagerankScore->at(0), 0.0001);
  ASSERT_NEAR(0.0708, node->pagerankScore->at(1), 0.0001);
  ASSERT_NEAR(0.08333, node->pagerankScore->at(2), 0.0001);
  ASSERT_NEAR(0.04583, node->pagerankScore->at(3), 0.0001);
  ASSERT_NEAR(0.08333, node->pagerankScore->at(4), 0.0001);


}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

