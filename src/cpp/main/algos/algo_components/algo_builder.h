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
#include "node_factory.h"
#include "../../common/components/deserializer_factory.h"

class AlgoBuilder {
public:
  Algo* buildFromConfig(unordered_map<string, string>* params);
  ~AlgoBuilder();
  Algo* getAlgo();
  Node* getNode();
  void setNodeFactory(INodeFactory*);
private:
  Util util_;
  Algo* createAlgoFromConfig(unordered_map<string, string>* params);
  Algo* algo_;
  Node* node_;
  Deserializer* deserializer_;
  SenderBuffer* senderBuffer_;
  SocketManager* socketManager_;
  MasterSocketManager* masterSocketManager_;
  StoreFromBinary* storeFromBinary_;
  INodeFactory* nodeFactory_;
};


#endif /* ALGO_BUIDER_H_ */
