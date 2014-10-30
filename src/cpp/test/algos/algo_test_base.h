/*
 * algo_test_base.h
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

#ifndef ALGO_TEST_BASE_H_
#define ALGO_TEST_BASE_H_

#include <gtest/gtest.h>
#include "../../main/common/util/logger_factory.h"
#include "../../main/algos/algo_components/test_master_builder.h"
#include "../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"

class AlgoTestBase: public ::testing::Test  {
protected:
  AlgoTestBase() {
    logger_ = &log4cpp::Category::getInstance(std::string("AlgoTestBase"));
    initSlavePort = 7001;
    host = "localhost";
    masterPort = 7000;
    sendLimit = 6000;
  }

  virtual ~AlgoTestBase() {}

  virtual void initParams(string nodeType) {
    logger_->info("Initing params.");
    numNodes_ = 0;
    slaveIndex_ = 0;
    slavePort_ = initSlavePort;
    std::stringstream ss;

    ss << slavePort_;
    params_["INIT_SLAVE_PORT"] = ss.str();
    ss.str("");

    params_["MASTER_PORT"] = util.intToString(masterPort);
    params_["SEND_LIMIT"] = util.intToString(sendLimit);
    params_["MASTER_HOST"] = host;
    params_["NODE_TYPE"] = nodeType;
    params_["INNER_MASTER_TYPE"] = nodeType;
    params_["DESERIALIZER_TYPE"] = nodeType;
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
      }
    }

    return retval;
  }

  EdgelistContainer* createContainer(vector<string> partString, long numSlaves) {
    long minNode = numNodes_;
    vector<vector<long> > part = toMatrix(partString);

    long numNodes = (long) part.size();

    EdgelistContainer* container = new EdgelistContainer;
    container->setMinnode(minNode);
    container->initContainers();
    for (int start = 0; start < (int) part.size(); ++start) {
      for (int i = 0; i < part[start].size(); ++i) {
        container->addEdge(start + minNode, part[start][i]);
      }
    }
    container->setFinish();

    return container;
  }

  void initLogger() {
    string debugLevel= "INFO";
    string appender = "CONSOLE";

    LoggerFactory::initLogger(debugLevel, appender, "");
    logger_ = &log4cpp::Category::getInstance(std::string("AlgoTestBase"));
    logger_->info("Logger started. Level %s.", debugLevel.c_str());
  }

  virtual void addNodeFactory(INodeFactory* nodeFactory, vector<string> part, long numSlaves) {
    nodeFactories_.push_back(nodeFactory);
    long minNode = numNodes_;
    long numNodes = (long) part.size();

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

  virtual void finalSetup() {
    std::stringstream ss;
    ss << numNodes_;
    params_["NUMLINE"] = ss.str();
    ss.str("");

    logger_->info("Setting number of slaves %d number of nodes %ld", slaveIndex_, numNodes_);
    ss << slaveIndex_;
    params_["NUM_SLAVES"] = ss.str();
    ss.str("");

    for (int i = 0; i < nodeParams_.size(); ++i) {
      nodeParams_[i]["NUMLINE"] = params_["NUMLINE"];
      nodeParams_[i]["NUM_SLAVES"] = params_["NUM_SLAVES"];
    }
  }

  void setUpBuilder() {
    masterBuilder_ = new TestMasterBuilder;
    masterBuilder_->setTestSlaveConfig(&slaves_);
  }

  vector<INodeFactory*> nodeFactories_;
  TestMasterBuilder* masterBuilder_;
  vector<Slave> slaves_;
  unordered_map<string, string> params_;
  vector<unordered_map<string, string> > nodeParams_;
  log4cpp::Category* logger_;
  long numNodes_;
  int slaveIndex_;
  int slavePort_;

  int initSlavePort;
  int masterPort;
  string host;
  int sendLimit;
  Util util;
};

#endif /* ALGO_TEST_BASE_H_ */
