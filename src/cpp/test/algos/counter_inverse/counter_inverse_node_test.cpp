/*
 * counter_inverse_node_test.cpp
 *
 *  Created on: 2014.09.26.
 *      Author: kisstom
 */

#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"
#include "../../../main/algos/algo_components/test_node_factories/test_counter_inverse_node_factory.h"

class CounterInverseNodeTest: public AlgoTestBase {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  CounterInverseNodeTest() {
  }

  virtual ~CounterInverseNodeTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
     initParams("COUNTER_INVERSE");
     initLogger();

     vector<string> part1;
     vector<string> part2;
     part1.push_back("1 2 3");
     part1.push_back("");
     part1.push_back("9");
     part1.push_back("8 10");
     part1.push_back("");
     part1.push_back("2 4 6");
     part1.push_back("");

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
  }

  void addNodeFactory(vector<string> partString, long numSlaves) {
    TestCounterInverseNodeFactory* nodeFactory = new TestCounterInverseNodeFactory;
    EdgelistContainer* container = createContainer(partString, numSlaves);
    nodeFactory->setEdgelistContainer(container);
    nodeFactory->setNumSlaves(numSlaves);
    AlgoTestBase::addNodeFactory(nodeFactory, partString, numSlaves);
  }
};


TEST_F(CounterInverseNodeTest, test) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
  cluster.init();
  cluster.start();

  CounterInverseNode* node = static_cast<CounterInverseNode*>(cluster.getNode(1));
  for (int i = 0; i < node->inversePartsEdges->size(); ++i) {
    node->logger_->info("%ld %ld %hd",
        node->inversePartsEdges->at(i).count,
        node->inversePartsEdges->at(i).to,
        node->inversePartsEdges->at(i).fromPartition);
  }




  ASSERT_EQ(5, (int) node->inversePartsEdges->size());
  ASSERT_EQ(2, (int) node->counter->at(0));
  ASSERT_EQ(0, (int) node->bounds->at(0));
  ASSERT_EQ(2, (int) node->bounds->at(1));
  ASSERT_EQ(4, (int) node->bounds->at(2));
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}





