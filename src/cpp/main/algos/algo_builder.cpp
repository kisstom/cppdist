/*
 * algo_builder.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "algo_builder.h"



Algo* AlgoBuilder::buildFromConfig(unordered_map<string, string>* params) {
	NodeFactory nodeFactory;
	DeserializerFactory deserializerFactory;

	algo_ = createAlgoFromConfig(params);

  node_ = nodeFactory.createNodeFromConfig(params);
	// should deserializer->setNode(node);
  deserializer_ = deserializerFactory.createDeserializerFromConfig(params, node_);
  algo_->setNode(node_);
  node_->setAlgo(algo_);

  senderBuffer_ = new SenderBuffer;
  algo_->setSenderBuffer(senderBuffer_);

  socketManager_ = new SocketManager;
  algo_->setSocketManager(socketManager_);

  storeFromBinary_ = new StoreFromBinary;
  storeFromBinary_->setDeserializer(deserializer_);

  algo_->setStoreFromBinary(storeFromBinary_);

  return algo_;
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
}

Algo* AlgoBuilder::createAlgoFromConfig(unordered_map<string, string>* params) {
	int master_port, init_slave_port, send_limit, num_slaves, slave_index, slave_port;
	string debugLevel;

	long all_node;
	sscanf((*params)["MASTER_PORT"].c_str(), "%d", &master_port);
	sscanf((*params)["INIT_SLAVE_PORT"].c_str(), "%d", &init_slave_port);
	sscanf((*params)["SEND_LIMIT"].c_str(), "%d", &send_limit);
	sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves);
	sscanf((*params)["NUMLINE"].c_str(), "%ld", &all_node);
	sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slave_index);

	char master_host[1024];
	//char logfile_n[1024];
	strcpy(master_host, (*params)["MASTER_HOST"].c_str());
	/*strcpy(logfile_n, (*params)["LOCAL_DIR"].c_str());
	char log[1024] = "/log";
	strcat(logfile_n, log);*/


  slave_port = init_slave_port + slave_index;

	return new Algo(master_host, master_port,
			slave_port, send_limit, all_node, num_slaves, slave_index);
}
