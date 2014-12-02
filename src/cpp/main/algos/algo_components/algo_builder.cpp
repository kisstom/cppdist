/*
 * algo_builder.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "algo_builder.h"
#include "../../common/components/socket/multicast_socket_manager.h"
#include <cstdlib>


Algo* AlgoBuilder::buildFromConfig(unordered_map<string, string>* params,
    vector<std::pair<string, string> >* hostAndPort) {
	DeserializerFactory deserializerFactory;

	algo_ = createAlgoFromConfig(params);
  node_ = nodeFactory_->createNodeFromConfig(params);

  deserializer_ = deserializerFactory.createDeserializerFromConfig(params, node_);
  algo_->setNode(node_);
  node_->setAlgo(algo_);

  senderBuffer_ = new SenderBuffer;
  algo_->setSenderBuffer(senderBuffer_);

  int initSlavePort = atoi((*params)["INIT_SLAVE_PORT"].c_str());
  int initCommPort = atoi((*params)["INIT_SLAVE_COMMUNICATION_PORT"].c_str());
  int slaveIndex = atoi((*params)["SLAVE_INDEX"].c_str());
  int numSlaves = atoi((*params)["NUM_SLAVES"].c_str());

  if (params->find("MULTI") != params->end()) {
    bool isMulticast = atoi((*params)["MULTI"].c_str());
    if (isMulticast) {
      socketManager_ = new MulticastSocketManager(slaveIndex, 1000,
            "225.0.0.", initSlavePort, numSlaves);
    } else {
      socketManager_ = new SocketManager(numSlaves, initSlavePort + slaveIndex);
    }
  } else {
    socketManager_ = new SocketManager(numSlaves, initSlavePort + slaveIndex);
  }

  algo_->setSocketManager(socketManager_);

  masterSocketManager_ = new MasterSocketManager;
  algo_->setMasterSocketManager(masterSocketManager_);

  socketManager_->setMasterSocketManager(masterSocketManager_);

  clientSocketManager_ = new ClientSocketManager(slaveIndex, numSlaves);
  clientSocketManager_->setMasterSocketManager(masterSocketManager_);

  clusterConfig = createClusterConfig(hostAndPort, initCommPort, numSlaves);
  clientSocketManager_->setClusterConfig(clusterConfig);
  algo_->setClientSocketManager(clientSocketManager_);

  storeFromBinary_ = new StoreFromBinary;
  storeFromBinary_->setDeserializer(deserializer_);

  algo_->setStoreFromBinary(storeFromBinary_);

  return algo_;
}

void AlgoBuilder::setNodeFactory(INodeFactory* nodeFactory) {
	nodeFactory_ = nodeFactory;
}

Algo* AlgoBuilder::getAlgo() {
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
  delete clientSocketManager_;
}

Algo* AlgoBuilder::createAlgoFromConfig(unordered_map<string, string>* params) {
  bool isMulticast = false;

  util_.checkParam(params, 4, "MASTER_PORT", "INIT_SLAVE_PORT","SEND_LIMIT", "NUM_SLAVES");
  util_.checkParam(params, 4, "NUMLINE", "SLAVE_INDEX", "NUM_NODES", "MIN_NODE");

	int master_port, init_slave_port, send_limit, num_slaves, slave_index, slave_port;
	string debugLevel;

	long all_node, min_node, num_nodes;
	sscanf((*params)["MASTER_PORT"].c_str(), "%d", &master_port);
	sscanf((*params)["INIT_SLAVE_PORT"].c_str(), "%d", &init_slave_port);
	sscanf((*params)["SEND_LIMIT"].c_str(), "%d", &send_limit);
	sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves);
	//logger_->info("number of dlaves ");
	sscanf((*params)["NUMLINE"].c_str(), "%ld", &all_node);
	sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slave_index);
	sscanf((*params)["NUM_NODES"].c_str(), "%ld", &num_nodes);
	sscanf((*params)["MIN_NODE"].c_str(), "%ld", &min_node);

	char master_host[1024];
	strcpy(master_host, (*params)["MASTER_HOST"].c_str());
  slave_port = init_slave_port + slave_index;

  if (params->find("MULTI") != params->end()) {
    isMulticast = atoi((*params)["MULTI"].c_str());
  }

	return new Algo(master_host, master_port,
			slave_port, send_limit, all_node, num_slaves, slave_index, num_nodes, min_node, isMulticast);
}

ClusterConfig* AlgoBuilder::createClusterConfig(
    vector<std::pair<string, string> >* hostAndPort, int initSlavePort, int numSlaves)  {
  ClusterConfig* config = new ClusterConfig;
  config->initClusterSize(numSlaves);

  int nodeIndex = 0;
  int nodeOnThisHost;
  char actIp[1024];

  for (vector<std::pair<string, string> >::iterator it = hostAndPort->begin();
      it != hostAndPort->end(); ++it) {
    nodeOnThisHost = atoi(it->second.c_str());
    util_.setIpByHost(it->first.c_str(), actIp);

    for (int i = 0; i < nodeOnThisHost; ++i) {
      config->setHost(actIp, nodeIndex);
      config->setPort(initSlavePort + nodeIndex, nodeIndex);
      ++nodeIndex;
    }
  }

  return config;
}
