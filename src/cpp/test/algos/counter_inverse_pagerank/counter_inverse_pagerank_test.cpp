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

     params_["MAX_ITER"] = 1;
     params_["DUMP"] = 0.9;

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

     addNodeFactory(part1, partIndices1, partitionBounds, 2);
     addNodeFactory(part2, partIndices2, partitionBounds, 2);

     setUpBuilder();
     finalSetup();
   }

  void initParams(string nodeType) {
    AlgoTestBase::initParams(nodeType);
  }

  void addNodeFactory(vector<string> pointerToCounters, vector<string> outPartitionIndices,
      vector<long>* partitionBound, long numSlaves) {
    TestCounterInversePagerankNodeFactory* nodeFactory = new TestCounterInversePagerankNodeFactory;
    EdgelistContainer* outpContainer = createContainer(outPartitionIndices, numSlaves);
    EdgelistContainer* counterContainer = createContainer(pointerToCounters, numSlaves);

    nodeFactory->setOutPartitionIndices(outpContainer);
    nodeFactory->setPartitionBound(partitionBound);
    nodeFactory->setPointerToCounters(counterContainer);
    nodeFactory->setNumSlaves(numSlaves);
    // TODO size is enough to pass
    AlgoTestBase::addNodeFactory(nodeFactory, pointerToCounters, numSlaves);
  }
};


TEST_F(CounterInversePagerankNodeTest, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
  cluster.init();
  cluster.start();

  CounterInversePagerankNode* node = static_cast<CounterInversePagerankNode*>(cluster.getNode(0));
  for (int i = 0; i < (int) node->pagerankScore->size(); ++i) {
    node->logger_->info("%d %lf", i, node->pagerankScore->at(i));
  }

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

