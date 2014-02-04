/*
 * cluster.cpp
 *
 *  Created on: 2014.02.03.
 *      Author: kisstom
 */

#include "cluster.h"

Cluster::Cluster(unordered_map<string, string>* params) {
	//numSlaves_ = numSlaves;
	params_ = params;
	logger_ = &log4cpp::Category::getInstance(std::string("Cluster"));
}

Cluster::~Cluster() {
  // TODO
}

void Cluster::init() {
	initMaster();
	initNodes();
}

Node* Cluster::getNode(int slaveIndex) {
	return builders_[slaveIndex]->getNode();
}

void Cluster::initNodes() {
	if (params_->find("NUM_SLAVES") == params_->end()) {
    logger_->error("Number of slaves are not set.");
    return;
	}

	sscanf((*params_)["NUM_SLAVES"].c_str(), "%d", &numSlaves_);

	for (int i = 0; i < numSlaves_; ++i) {
		initNode(i);
	}
}

void Cluster::initNode(int nodeId) {
	char strNodeId[1024];
	sprintf(strNodeId, "%d", nodeId);

	(*params_)["SLAVE_INDEX"] = string(strNodeId);
	AlgoBuilder* builder = new AlgoBuilder;
	builder->buildFromConfig(params_);
	builders_.push_back(builder);
}

void Cluster::initMaster() {
	TestMasterBuilder builder;
	// TODO set slaves
	builder.setTestSlaveConfig(NULL);
	master_ = builder.buildFromConfig(params_);
}


void Cluster::start() {
	MainThread* masterThread;
	Algo* algo;

	vector<MainThread*> nodes;
  if (master_->setUp()) {
  	masterThread = new MainThread(master_);
  	masterThread->start();
  }

  for (int slaveIndex = 0; slaveIndex < numSlaves_; ++slaveIndex) {
  	algo = builders_[slaveIndex]->getAlgo();
  	if (algo->setUp()) {
  	  MainThread* nodeThread = new MainThread(algo);
  	  nodeThread->start();
  	  nodes.push_back(nodeThread);
  	}
  }

  for (int i = 0; i < nodes.size(); ++i) {
  	nodes[i]->waitForThread();
  }

  masterThread->waitForThread();
}


