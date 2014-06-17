/*
 * algo_test_base.h
 *
 *  Created on: 2014.06.16.
 *      Author: kisstom
 */

#ifndef ALGO_TEST_BASE_H_
#define ALGO_TEST_BASE_H_

#include <gtest/gtest.h>

#include "../../../main/algos/cluster.h"
#include "../../../main/algos/test_master_builder.h"
#include "../../../main/algos/test_simrank_odd_even_node_factory.h"
#include "../../../main/common/util/logger_factory.h"
#include <stdlib.h>
#include <sstream>

using std::tr1::unordered_map;

namespace {

class AlgoTestBase: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  AlgoTestBase() {
  }

  virtual ~AlgoTestBase() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void addFactory() = 0;

  void initParams(string type) {
      numNodes_ = 0;
      slaveIndex_ = 0;
      slavePort_ = 7001;
      std::stringstream ss;

      ss << slavePort_;
      params_["INIT_SLAVE_PORT"] = ss.str();
      ss.str("");

      params_["MASTER_PORT"] = "7000";
      params_["SEND_LIMIT"] = "6000";
      params_["MASTER_HOST"] = "localhost";
      params_["NODE_TYPE"] = type;
      params_["INNER_MASTER_TYPE"] = type;
      params_["DESERIALIZER_TYPE"] = type;

    }



  void addPartition(vector<string> partString, long numSlaves) {
    long minNode = numNodes_;
    vector<vector<long> > part = toMatrix(partString);

    long numNodes = (long) part.size();

    addFactory();

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
  }

  void initLogger() {
    string debugLevel= "INFO";
    string appender = "CONSOLE";

    LoggerFactory::initLogger(debugLevel, appender, "");
    logger_ = &log4cpp::Category::getInstance(std::string("AlgoTestBase"));
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
};

}


#endif /* ALGO_TEST_BASE_H_ */
