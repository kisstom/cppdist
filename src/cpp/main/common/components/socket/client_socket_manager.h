#ifndef CLIENT_SOCKET_MANAGER_H_
#define CLIENT_SOCKET_MANAGER_H_

#include "cluster_config.h"
#include "log4cpp/Category.hh"
#include "zmq_sockets/zmq.hpp"
#include "master_socket_manager.h"
#include "../../../../main/algos/algo_components/runnable.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class ClientSocketManager: public Runnable {
public:
  ClientSocketManager(int selfIndex, int clusterSize);
  void publishEndSignal();
  void run();
  bool setUp();

  void setClusterConfig(ClusterConfig*);
  void setMasterSocketManager(MasterSocketManager*);
  bool isFinished();
  void resetFinishCounter();

  int buildFDSet();
  int findFdIsSet();
  void buildPoll();
  void destroyPoll();

  ~ClientSocketManager();
private:

  void incrementFinishCounter();
  void initSubscribes();
  void initPublisher();

  int finishCounter;
  int numCluster;
  int startClienPort;
  int selfIndex;
  int sendHWM;
  fd_set fdSet;

  MasterSocketManager* masterSocketManager;

  ClusterConfig* clusterConfig;
  zmq::context_t* context;
  zmq::socket_t* publisherSocket;
  vector<zmq::socket_t*> listenerSockets;

  log4cpp::Category* logger;
};


#endif  // CLIENT_SOCKET_MANAGER_H_
