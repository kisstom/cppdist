/*
 * simrank_odd_even_test.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include <gtest/gtest.h>

#include "../../../main/algos/cluster.h"
#include "../../../main/algos/test_master_builder.h"
#include "../../../main/algos/test_simrank_odd_even_node_factory.h"


using std::tr1::unordered_map;

namespace {

class SimrankOddEvenTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SimrankOddEvenTest() {
  }

  virtual ~SimrankOddEvenTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  void setUpFirst() {
		EdgelistContainer* container = new EdgelistContainer;
		container->setMinnode(0);
		container->initContainers();
		container->addEdge(0, 1);
		container->setFinish();

		vector<list<long*> > fpStarts;
		list<long*> pathes;
		long* path = new long[11];
		path[0] = 0;
		path[1] = 0;
		pathes.push_back(path);
		fpStarts.push_back(pathes);

		TestSimrankOddEvenNodeFactory* nodeFactory =
				new TestSimrankOddEvenNodeFactory;
		nodeFactory->setEdgelistContainer(container);
		nodeFactory->setFingerprints(fpStarts);

		nodeFactories_.push_back(nodeFactory);
  }

  void setUpSecond() {
		EdgelistContainer* container = new EdgelistContainer;
		container->setMinnode(1);
		container->initContainers();
		container->addEdge(1, 0);
		container->setFinish();

		vector<list<long*> > fpStarts;
		list<long*> pathes;
		long* path = new long[11];
		path[0] = 1;
		path[1] = 1;
		pathes.push_back(path);
		fpStarts.push_back(pathes);

		TestSimrankOddEvenNodeFactory* nodeFactory =
				new TestSimrankOddEvenNodeFactory;
		nodeFactory->setEdgelistContainer(container);
		nodeFactory->setFingerprints(fpStarts);

		nodeFactories_.push_back(nodeFactory);
  }

  virtual void SetUp() {
  	setUpFirst();
  	setUpSecond();
  }

  virtual void TearDown() {
  }

  vector<INodeFactory*> nodeFactories_;
  IMasterBuilder* masterBuilder_;

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(SimrankOddEvenTest, testRun) {
	unordered_map<string, string> params;
  //long all_node, num_

	params["MASTER_PORT"] = "7000";
	params["INIT_SLAVE_PORT"] = "7001";
	params["SEND_LIMIT"] = "6000";
	params["NUM_SLAVES"] = "2";
	// numline variable
	params["NUMLINE"] = "2";
	params["MASTER_HOST"] = "localhost";

	params["NODE_TYPE"] = "SIMRANK_ODD_EVEN";
	params["NUM_PATHES"] = "1";
	params["PATH_LEN"] = "10";
	params["RANDOM_TYPE"] = "PSEUDO";
	params["SEED"] = "13";
	//params["NUM_NODES"] = "1";
	//params["MIN_NODE"] = "0";
	//params["NEXT_MIN_NODE"] = "1";

	params["INNER_MASTER_TYPE"] = "SIMRANK_ODD_EVEN";
	params["APPENDER"] = "CONSOLE";



	vector<Slave*> slaves;
	//slaves.push_back;
	TestMasterBuilder* masterBuilder = new TestMasterBuilder;
  //masterBuilder->setSlaves();

	Cluster cluster(&params, nodeFactories_, masterBuilder);
	cluster.init();
	cluster.start();
}
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


