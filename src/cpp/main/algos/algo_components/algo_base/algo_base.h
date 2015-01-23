#ifndef ALGO_BASE_H_
#define ALGO_BASE_H_

#include <tr1/unordered_map>
#include <vector>

#include <gtest/gtest_prod.h>
#include "../node.h"
#include "../runnable.h"
#include "../../../common/components/sender_buffer.h"
#include "../../../common/components/socket/client_socket_manager.h"
#include "../../../common/components/socket/socket_manager.h"
#include "../../../common/components/socket/master_socket_manager.h"
#include "../../../common/components/store_from_binary.h"

#include <sstream>
#include "log4cpp/PatternLayout.hh"
#include <limits.h>

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

using std::tr1::unordered_map;

class Node;

class AlgoBase: public Runnable {
public:
  AlgoBase(unordered_map<string, string>* _params);

  virtual bool setUp() = 0;
  virtual void run() = 0;
  virtual void receiver() = 0;
  virtual void runThreads() = 0;
  virtual void sendAndSignal(int) = 0;

  // Getters.
  int getSlaveIndex();
  int getNumSlaves();

  // Setters.
  void setNode(Node * node);
  void setSocketManager(ISocketManager* manager);
  void setSenderBuffer(SenderBuffer* senderBuffer);
  void setMasterSocketManager(MasterSocketManager* manager);
  void setStoreFromBinary(StoreFromBinary* storeFromBinary);

  // Calling Node object.
  void final();
  bool storeFromBinary(int socket_index);

  virtual ~AlgoBase();

protected:
  void initFromParams(unordered_map<string, string>*);

  char master_host_[1024];
  int master_port_;
  int slave_port_;

  int slave_index_;
  int num_slaves_;
  int send_limit_;

  ISocketManager* socketManager_;
  MasterSocketManager* masterSocketManager_;
  SenderBuffer* senderBuffer_;
  Node* node_;
  StoreFromBinary* storeFromBinary_;
  log4cpp::Category* logger_;
  friend class SimpleMockTestSetup;
  friend class SimpleMockTestMultiSetup;
};


#endif  // ALGO_BASE_H_
