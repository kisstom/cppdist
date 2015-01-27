#ifndef NEW_ALGO_TEST_BASE_H_
#define NEW_ALGO_TEST_BASE_H_

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../../main/algos/algo_components/inode_factory.h"
#include "../../main/common/util/logger_factory.h"
#include "log4cpp/Category.hh"

using std::string;
using std::vector;

class NewAlgoTestBase: public ::testing::Test  {
protected:
  NewAlgoTestBase();

  void addNodeFactory(INodeFactory*);
  void addConfigParam(string key, string value);
  void initCluster();
  void setupCluster();
  void runCluster();
  void deleteCluster();
  void setDefaultConfig();
  void initLogger();

  Cluster* cluster;
  log4cpp::Category* logger_;
  Util util;
private:
  vector<INodeFactory*> nodeFactories;
  int numSlaves;
  unordered_map<string, string> params;
};

NewAlgoTestBase::NewAlgoTestBase() {
  numSlaves = 0;
}

void NewAlgoTestBase::setDefaultConfig() {
  params["INIT_SLAVE_COMMUNICATION_PORT"] = "8001";
  params["INIT_SLAVE_PORT"] = "7001";
  params["MASTER_PORT"] = "7000";
  params["SEND_LIMIT"] = "6000";
  params["MASTER_HOST"] = "localhost";
}

void NewAlgoTestBase::initLogger() {
   string debugLevel= "INFO";
   string appender = "CONSOLE";

   LoggerFactory::initLogger(debugLevel, appender, "");
   logger_ = &log4cpp::Category::getInstance(std::string("NewAlgoTestBase"));
   logger_->info("Logger started. Level %s.", debugLevel.c_str());
 }

void NewAlgoTestBase::addConfigParam(string key, string value) {
  params[key] = value;
}


void NewAlgoTestBase::addNodeFactory(INodeFactory* nodeFactory) {
  nodeFactories.push_back(nodeFactory);
  ++numSlaves;
}

void NewAlgoTestBase::initCluster() {
  setDefaultConfig();
  vector<std::pair<string, string> > clusterNodeParams;
  clusterNodeParams.push_back(std::make_pair<string, string>("localhost", util.intToString(numSlaves)));
  params["NUM_SLAVES"] = util.intToString(numSlaves);
  cluster = new Cluster(params, clusterNodeParams, nodeFactories);
  cluster->init();
}

void NewAlgoTestBase::setupCluster() {
  cluster->setUp();
}

void NewAlgoTestBase::runCluster() {
  cluster->start();
}

void NewAlgoTestBase::deleteCluster() {
  nodeFactories.clear();
  numSlaves = 0;
  params.clear();
  delete cluster;
}

#endif  // NEW_ALGO_TEST_BASE_H_
