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
#include "../../../main/common/util/logger_factory.h"
#include <stdlib.h>
#include <sstream>

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

  void addPartition(vector<string> partString, long minNode) {
  	vector<vector<long> > part = toMatrix(partString);

  	long numNodes = (int) part.size();

  	vector<list<long*> > fpStarts;
  	fpStarts.resize(numPathes_);

  	EdgelistContainer* container = new EdgelistContainer;
  	container->setMinnode(minNode);
  	container->initContainers();
  	for (long start = 0; start < (long) part.size(); ++start) {
  		for (int i = 0; (int) part[start].size(); ++i) {
  			container->addEdge(start + minNode, part[(int)start][i]);
  		}

  		for (int pathI = 0; pathI < numPathes_; ++pathI) {
  			long* path = new long[pathLen_ + 1];
  			path[0] = start;
  			path[1] = start;
  			fpStarts[pathI].push_back(path);
  		}
  	}
  	container->setFinish();

		TestSimrankOddEvenNodeFactory* nodeFactory =
				new TestSimrankOddEvenNodeFactory;
		nodeFactory->setEdgelistContainer(container);
		nodeFactory->setFingerprints(fpStarts);
		nodeFactories_.push_back(nodeFactory);

		Slave slave;
		slave.minNode = minNode;
		slave.numNode = numNodes;
		numNodes_ += slave.numNode;
		slave.port = slavePort_++;
		slaves_.push_back(slave);
		params_["SLAVE_INDEX"] = slaveIndex_++;
		params_["NUM_NODES"] = numNodes;
		params_["MIN_NODE"] = minNode;

		nodeParams_.push_back(params_);
  }

  void setUpBuilder() {
  	masterBuilder_ = new TestMasterBuilder;
    masterBuilder_->setTestSlaveConfig(&slaves_);
  }

  void initParams() {
  	pathLen_ = 2;
  	numPathes_ = 1;
  	numNodes_ = 0;
  	slaveIndex_ = 0;
  	slavePort_ = 7001;
    std::stringstream ss;

    ss << slavePort_;
  	params_["INIT_SLAVE_PORT"] = ss.str();
  	ss.str("");

    ss << pathLen_;
    params_["PATH_LEN"] = ss.str();
    ss.str("");

    ss << numPathes_;
    params_["NUM_PATHES"] = ss.str();
    ss.str("");

		params_["MASTER_PORT"] = "7000";
		params_["SEND_LIMIT"] = "6000";
		params_["MASTER_HOST"] = "localhost";
		params_["NODE_TYPE"] = "SIMRANK_ODD_EVEN";
		params_["RANDOM_TYPE"] = "PSEUDO";
		params_["SEED"] = "13";
		params_["INNER_MASTER_TYPE"] = "SIMRANK_ODD_EVEN";
	  params_["DESERIALIZER_TYPE"] = "SIMRANK_ODD_EVEN";
  }

  void finalSetup() {
  	std::stringstream ss;
  	ss << numNodes_;
  	params_["NUMLINE"] = ss.str();
  	ss.str("");

  	ss << slaveIndex_;
    params_["NUM_SLAVES"] = ss.str();
    ss.str("");
  }

  void initLogger() {
  	string debugLevel= "INFO";
  	string appender = "CONSOLE";

  	LoggerFactory::initLogger(debugLevel, appender, "");
  	logger_ = &log4cpp::Category::getInstance(std::string("SimrankOddEvenTest"));
  	logger_->info("Logger started. Level %s.", debugLevel.c_str());
  }

  vector<vector<long> > toMatrix(vector<string> stringM) {
  	vector<vector<long> > retval;
  	retval.resize(stringM.size());
  	char node[1024];

  	for (int i = 0; i < (int) stringM.size(); ++i) {
  		std::stringstream row(stringM[i]);
  		while (row.peek() != EOF) {
  			row >> node;
        retval[i].push_back(atol(node));
        logger_->info("%ld ", atol(node));
  		}
  	}

  	return retval;
  }

  virtual void SetUp() {
  	initParams();
  	initLogger();

  	vector<string> part1;
    part1.push_back("");
    part1.push_back("0 2");
    addPartition(part1, 0);

  	vector<string> part2;
    part2.push_back("0 3");
    part2.push_back("0 1");
    addPartition(part2, 2);

  	setUpBuilder();
  	finalSetup();
  }

  virtual void TearDown() {
  }

  vector<INodeFactory*> nodeFactories_;
  TestMasterBuilder* masterBuilder_;
  vector<Slave> slaves_;
  unordered_map<string, string> params_;
  vector<unordered_map<string, string> > nodeParams_;
  log4cpp::Category* logger_;
  int pathLen_;
  int numPathes_;
  long numNodes_;
  int slaveIndex_;
  int slavePort_;
  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(SimrankOddEvenTest, testRun) {
	Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
	cluster.init();
	cluster.start();

	/*HashPseudoRandom random(13);
  for (short i = 0; i < pathLen_; ++i) {
  	for (long node = 1; node < numNodes_; ++node) {
  		printf("%d ", random.get(0, i, node));
  	}
  	printf("\n");
  }*/


  SimrankOddEvenNode* node = static_cast<SimrankOddEvenNode*>(cluster.getNode(0));
	vector<list<long*> >* pathes = node->getPathes();
	for (int i = 0; i < pathes->size(); ++i) {
		for (list<long*>::iterator it = (*pathes)[i].begin(); it != (*pathes)[i].end(); ++it) {
			for (int j = 0; j <= pathLen_; ++j) {
				printf("%d ", (*it)[j]);
			}
			printf("\n");
		}
	}

	SimrankOddEvenNode* node2 =
			static_cast<SimrankOddEvenNode*>(cluster.getNode(1));
	pathes = node2->getPathes();
	for (int i = 0; i < pathes->size(); ++i) {
		for (list<long*>::iterator it = (*pathes)[i].begin();
				it != (*pathes)[i].end(); ++it) {
			for (int j = 0; j <= pathLen_; ++j) {
				printf("%d ", (*it)[j]);
			}
			printf("\n");
		}
	}

}
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


