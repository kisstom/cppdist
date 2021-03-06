/*
 * algo_buider.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef ALGO_BUILDER_H_
#define ALGO_BUILDER_H_

#include "../../common/util/cfg_reader.h"
#include "../../common/util/config/INIReader.h"
#include "node_factory.h"
#include "../../common/components/deserializer_factory.h"
#include "../../common/components/socket/cluster_config.h"
#include "factories/ialgo_factory.h"
#include "factories/socket_manager_factory.h"
#include "factories/sender_buffer_factory.h"
#include "factories/store_from_binary_factory.h"
#include "factories/client_socket_manager_factory.h"

class AlgoBuilder {
public:
  AlgoBuilder();
  AlgoBase* buildFromConfig(unordered_map<string, string>* params,vector<std::pair<string, string> >* hostAndPort);
  ~AlgoBuilder();
  AlgoBase* getAlgo();
  Node* getNode();

  void setNodeFactory(INodeFactory*);
  void setAlgoFactory(IAlgoFactory*);
private:
  Util util_;
  AlgoBase* createAlgoFromConfig(unordered_map<string, string>* params);
  AlgoBase* algo_;
  Node* node_;
  INodeFactory* nodeFactory_;
  IAlgoFactory* algoFactory_;

  Deserializer* deserializer_;
  SenderBuffer* senderBuffer_;
  ISocketManager* socketManager_;
  MasterSocketManager* masterSocketManager_;
  ClientSocketManager* clientSocketManager_;
  StoreFromBinary* storeFromBinary_;


  DeserializerFactory deserializerFactory;
  SocketManagerFactory socketManagerFactory;
  SenderBufferFactory senderBufferFactory;
  StoreFromBinaryFactory storeFromBinaryFactory;
  ClientSocketManagerFactory clientSocketManagerFactory;
};


#endif /* ALGO_BUIDER_H_ */
