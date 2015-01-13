/*
 * algo_builder.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "algo_builder.h"
#include "factories/algo_factory.h"
#include "algo_base/three_threaded_algo.h"
#include "../../common/components/socket/multicast_socket_manager.h"
#include <cstdlib>
#include <cmath>

AlgoBuilder::AlgoBuilder() {
  algo_ = NULL;
  node_ = NULL;
  nodeFactory_ = NULL;
  algoFactory_ = NULL;

  deserializer_ = NULL;
  senderBuffer_ = NULL;
  socketManager_ = NULL;
  masterSocketManager_ = NULL;
  clientSocketManager_ = NULL;
  storeFromBinary_ = NULL;
}

AlgoBase* AlgoBuilder::buildFromConfig(unordered_map<string, string>* params,
    vector<std::pair<string, string> >* hostAndPort) {

	int init_slave_port, slave_index, slave_port;
	sscanf((*params)["INIT_SLAVE_PORT"].c_str(), "%d", &init_slave_port);
	sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slave_index);
	slave_port = init_slave_port + slave_index;

	algo_ = algoFactory_->createAlgo(params);
  node_ = nodeFactory_->createNodeFromConfig(params);

  deserializer_ = deserializerFactory.createDeserializerFromConfig(params, node_);
  algo_->setNode(node_);
  node_->setAlgo(algo_);

  senderBuffer_ = senderBufferFactory.createSenderBuffer(params);
  algo_->setSenderBuffer(senderBuffer_);

  socketManager_ = socketManagerFactory.createSocketManager(params);
  algo_->setSocketManager(socketManager_);

  masterSocketManager_ = new MasterSocketManager;
  masterSocketManager_->setPort(slave_port);
  algo_->setMasterSocketManager(masterSocketManager_);
  socketManager_->setMasterSocketManager(masterSocketManager_);

  storeFromBinary_ = storeFromBinaryFactory.createStoreFromBinary(params);
  storeFromBinary_->setDeserializer(deserializer_);

  if (params->find("MULTI") != params->end() && atoi((*params)["MULTI"].c_str()) == 1) {
    clientSocketManager_ = clientSocketManagerFactory.
        createClientSocketManager(params, hostAndPort);
    clientSocketManager_->setMasterSocketManager(masterSocketManager_);
    ThreeThreadedAlgo* tmpAlgo = dynamic_cast<ThreeThreadedAlgo*>(algo_);
    tmpAlgo->setClientSocketManager(clientSocketManager_);
  }

  algo_->setStoreFromBinary(storeFromBinary_);

  return algo_;
}

void AlgoBuilder::setNodeFactory(INodeFactory* nodeFactory) {
	nodeFactory_ = nodeFactory;
}

void AlgoBuilder::setAlgoFactory(IAlgoFactory* algoFactory) {
  algoFactory_ = algoFactory;
}

AlgoBase* AlgoBuilder::getAlgo() {
	return algo_;
}

Node* AlgoBuilder::getNode() {
	return node_;
}

AlgoBuilder::~AlgoBuilder() {
  delete senderBuffer_;
  delete socketManager_;
  delete storeFromBinary_;
  delete deserializer_;
  delete node_;
  delete algo_;
  delete masterSocketManager_;
  if (clientSocketManager_) delete clientSocketManager_;
}

