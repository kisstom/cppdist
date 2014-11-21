/*
 * simrank_test_builder.cpp
 *
 *  Created on: 2014.02.24.
 *      Author: kisstom
 */
#include <gtest/gtest.h>

#include "../../../main/algos/algo_components/cluster.h"
#include "../../../main/algos/algo_components/test_master_builder.h"
#include "../../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"
#include "../../../main/common/util/logger_factory.h"
#include <stdlib.h>
#include <sstream>

using std::tr1::unordered_map;

namespace {

class SimrankTestBase: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SimrankTestBase() {
  }

  virtual ~SimrankTestBase() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void initParams(string nodeType) {
      //pathLen_ = 10;
      //numPathes_ = 2;
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
      params_["NODE_TYPE"] = nodeType;
      params_["RANDOM_TYPE"] = "PSEUDO";
      params_["SEED"] = "13";
      params_["INNER_MASTER_TYPE"] = nodeType;
      params_["DESERIALIZER_TYPE"] = nodeType;
      expectedPathes_.resize(numPathes_);
  }

  void addPartition(vector<string> partString, long numSlaves) {
  	long minNode = numNodes_;
  	vector<vector<long> > part = toMatrix(partString);

  	long numNodes = (long) part.size();

  	vector<list<long*> > fpStarts;
  	fpStarts.resize(numPathes_);

  	EdgelistContainer* container = new EdgelistContainer;
  	container->setMinnode(minNode);
  	container->initContainers();
  	for (int start = 0; start < (int) part.size(); ++start) {
  		for (int i = 0; i < part[start].size(); ++i) {
  			container->addEdge(start + minNode, part[start][i]);
  		}

  		for (int pathI = 0; pathI < numPathes_; ++pathI) {
  			long* path = new long[pathLen_ + 1];
  			std::fill_n(path, pathLen_ + 1, -1);
  			path[0] = start + minNode;
  			path[1] = start + minNode;
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

		std::stringstream ss;
		ss << numNodes;
		params_["NUM_NODES"] = ss.str();
		ss.str("");

		ss << slaveIndex_;
		params_["SLAVE_INDEX"] = ss.str();
		ss.str("");
		++slaveIndex_;

		ss << numSlaves;
		params_["NUM_SLAVES"] = ss.str();
		ss.str("");

		long nextMinNode = minNode + numNodes;
		ss << nextMinNode;
		params_["NEXT_MIN_NODE"] = ss.str();
		ss.str("");

		ss << minNode;
		params_["MIN_NODE"] = ss.str();

		nodeParams_.push_back(params_);
		clusterNodeParams.push_back(std::make_pair<string, string>("localhost", params_["SLAVE_INDEX"]));
  }

  void setUpBuilder() {
  	masterBuilder_ = new TestMasterBuilder;
    masterBuilder_->setTestSlaveConfig(&slaves_);
  }

  void finalSetup() {
  	std::stringstream ss;
  	ss << numNodes_;
  	params_["NUMLINE"] = ss.str();
  	ss.str("");

  	logger_->info("Setting number of slaves %d", slaveIndex_);
  	ss << slaveIndex_;
    params_["NUM_SLAVES"] = ss.str();
    ss.str("");

    for (int i = 0; i < nodeParams_.size(); ++i) {
      nodeParams_[i]["NUMLINE"] = params_["NUMLINE"];
      nodeParams_[i]["NUM_SLAVES"] = params_["NUM_SLAVES"];
    }
  }

  void initLogger() {
  	string debugLevel= "EMERG";
  	string appender = "CONSOLE";

  	LoggerFactory::initLogger(debugLevel, appender, "");
  	logger_ = &log4cpp::Category::getInstance(std::string("SimrankTestBase"));
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
        //logger_->info("%ld ", atol(node));
  		}
  	}

  	return retval;
  }

  void addExpectedPath(int fpIndex, string expected) {
		std::stringstream row(expected);
		long * path = new long[pathLen_ + 1];
		std::fill_n(path, pathLen_ + 1, -1);
		int index = 0;
		long node;
		while (row.peek() != EOF) {
			row >> node;
			path[index] = node;
			++index;
		}
		expectedPathes_[fpIndex].push_back(path);
  }

  /*virtual void SetUp() {

  }*/

  void printPath(long* path) {
  	int i = 0;
  	while (path[i] != -1 && i <= pathLen_) {
  		printf(" %ld", path[i]);
  		++i;
  	}
  	printf("\n");
  }

  bool eq(long* path, long* other) {
  	int i = 0;
  	while (i < pathLen_ + 1) {
  		if (path[i] != other[i]) return false;
  		if (path[i] < 0) return true;
  		++i;
  	}

    return true;
  }

  bool in(long* path, vector<long*> fp) {
  	for (vector<long*>::iterator it = fp.begin(); it != fp.end(); ++it) {
  		if (eq(path, *it)) {
  			return true;
  		}
  	}

  	return false;
  }

  void checkContains(vector<vector<long*> >* th, vector<vector<long*> >* oth) {

  	for (int i = 0; i < th->size(); ++i) {
  		for (vector<long*>::iterator it = (*th)[i].begin(); it != (*th)[i].end(); ++it) {
  			ASSERT_TRUE(in(*it, (*oth)[i]));
  		}
  	}
  }

  void check(vector<vector<long*> >* th, vector<vector<long*> >* oth) {
  	checkContains(th, oth);
  	checkContains(oth, th);
  }

  virtual void TearDown() {
  }

  vector<INodeFactory*> nodeFactories_;
  TestMasterBuilder* masterBuilder_;
  vector<Slave> slaves_;
  unordered_map<string, string> params_;
  vector<unordered_map<string, string> > nodeParams_;
  vector<std::pair<string, string> > clusterNodeParams;

  log4cpp::Category* logger_;
  int pathLen_;
  int numPathes_;
  long numNodes_;
  int slaveIndex_;
  int slavePort_;

  vector<vector<long*> >* concat_;
  vector<vector<long*> > expectedPathes_;
  // Objects declared here can be used by all tests in the test case for Foo.
};

}





