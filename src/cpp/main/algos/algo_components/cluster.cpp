/*
 * cluster.cpp
 *
 *  Created on: 2014.02.03.
 *      Author: kisstom
 */

#include "cluster.h"

Cluster::Cluster(unordered_map<string, string>* params, vector<unordered_map<string, string> >* nodeParams,
		vector<INodeFactory*> nodeFactories, IMasterBuilder* masterBuilder,
		vector<std::pair<string, string> >* clusterNodeParams) {
	//numSlaves_ = numSlaves;
	sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &numSlaves_);
	params_ = params;
	nodeParams_ = nodeParams;
	clusterNodeParams_ = clusterNodeParams;
	nodeFactories_ = nodeFactories;
	masterBuilder_ = masterBuilder;
	logger_ = &log4cpp::Category::getInstance(std::string("Cluster"));
}

Cluster::~Cluster() {
  delete masterBuilder_;
  for (int i = 0; i < builders_.size(); ++i) {
    delete builders_[i];
  }
}

void Cluster::init() {
	initMaster();
	initNodes();
}

Node* Cluster::getNode(int slaveIndex) {
	return builders_[slaveIndex]->getNode();
}

AlgoBase* Cluster::getAlgo(int slaveIndex) {
  return builders_[slaveIndex]->getAlgo();
}

void Cluster::initNodes() {
  logger_->info("Initing nodes.");
	for (int i = 0; i < numSlaves_; ++i) {
		initNode(i);
	}
}

void Cluster::initNode(int nodeId) {
	logger_->info("Initing node %d.", nodeId);
	AlgoBuilder* builder = new AlgoBuilder;

	INodeFactory* nodeFactory = nodeFactories_[nodeId];

	builder->setNodeFactory(nodeFactory);
	builder->buildFromConfig(&(nodeParams_->at(nodeId)), clusterNodeParams_);
	builders_.push_back(builder);
}

void Cluster::initMaster() {
	master_ = masterBuilder_->buildFromConfig(params_);
}

void Cluster::setUp() {
  logger_->info("Starting cluster.");
  SetupThread* masterThread = NULL;
  AlgoBase* algo = NULL;

  vector<SetupThread*> nodes;
  masterThread = new SetupThread(master_);
  masterThread->start();
  logger_->info("Master started.");

  for (int slaveIndex = 0; slaveIndex < numSlaves_; ++slaveIndex) {
    logger_->info("Starting node %d.", slaveIndex);
    algo = builders_[slaveIndex]->getAlgo();
    SetupThread* nodeThread = new SetupThread(algo);
    nodes.push_back(nodeThread);
    nodeThread->start();
    logger_->info("Node %d started.", slaveIndex);
  }

  for (int i = 0; i < nodes.size(); ++i) {
    nodes[i]->waitForThread();
  }

  masterThread->waitForThread();
}


void Cluster::start() {
	logger_->info("Starting cluster.");
	MainThread* masterThread;
	AlgoBase* algo;

	vector<MainThread*> nodes;
	masterThread = new MainThread(master_);
	masterThread->start();
	logger_->info("Master started.");

  for (int slaveIndex = 0; slaveIndex < numSlaves_; ++slaveIndex) {
  	logger_->info("Starting node %d.", slaveIndex);
  	algo = builders_[slaveIndex]->getAlgo();
  	MainThread* nodeThread = new MainThread(algo);
  	nodes.push_back(nodeThread);
  	nodeThread->start();
  	logger_->info("Node %d started.", slaveIndex);
  }

  for (int i = 0; i < nodes.size(); ++i) {
  	nodes[i]->waitForThread();
  }

  masterThread->waitForThread();
}


