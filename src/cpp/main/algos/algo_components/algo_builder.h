/*
 * algo_buider.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef ALGO_BUILDER_H_
#define ALGO_BUILDER_H_

#include "algo.h"
#include "../../common/util/cfg_reader.h"
#include "../../common/util/config/INIReader.h"
#include "node_factory.h"
#include "../../common/components/deserializer_factory.h"
#include "../../common/components/socket/cluster_config.h"

class AlgoBuilder {
public:
  AlgoBase* buildFromConfig(unordered_map<string, string>* params,vector<std::pair<string, string> >* hostAndPort);
  ~AlgoBuilder();
  AlgoBase* getAlgo();
  Node* getNode();
  ClusterConfig* createClusterConfig(vector<std::pair<string, string> >* params, int, int);
  void setNodeFactory(INodeFactory*);
private:
  Util util_;
  AlgoBase* createAlgoFromConfig(unordered_map<string, string>* params);
  AlgoBase* algo_;
  Node* node_;
  Deserializer* deserializer_;
  SenderBuffer* senderBuffer_;
  ISocketManager* socketManager_;
  MasterSocketManager* masterSocketManager_;
  ClientSocketManager* clientSocketManager_;
  StoreFromBinary* storeFromBinary_;
  INodeFactory* nodeFactory_;
};


#endif /* ALGO_BUIDER_H_ */
