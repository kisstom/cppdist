#ifndef CLIENT_SOCKET_MANAGER_H_
#define CLIENT_SOCKET_MANAGER_H_

#include "cluster_config.h"
#include "log4cpp/Category.hh"
#include "zmq_sockets/zmq.hpp"
#include "master_socket_manager.h"

class ClientSocketManager {
public:
  ClientSocketManager(int selfIndex, int clusterSize);
  void publishEndSignal();
  void run();
  void setUp();

  void setClusterConfig(ClusterConfig*);
  void setMasterSocketManager(MasterSocketManager*);

  void resetFinishCounter();
  bool isFinished();
  void incrementFinishCounter();
  void initSubscribes();
  void initPublisher();

private:
  int finishCounter;
  int numCluster;
  int startClienPort;
  int selfIndex;
  int sendHWM;

  MasterSocketManager* masterSocketManager;

  ClusterConfig* clusterConfig;
  zmq::context_t* context;
  zmq::socket_t* publisherSocket;
  zmq::socket_t* listenerSocket;
  log4cpp::Category* logger;
};


#endif  // CLIENT_SOCKET_MANAGER_H_
