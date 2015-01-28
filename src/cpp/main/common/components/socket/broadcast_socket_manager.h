#ifndef BROADCAST_SOCKET_MANAGER_H_
#define BROADCAST_SOCKET_MANAGER_H_

#include "isocket/isocket_manager.h"
#include "UDPMulticastPublisher.h"
#include "UDPMulticastReceiver.h"
#include "isocket/isocket_manager.h"
#include "master_socket_manager.h"
#include "log4cpp/Category.hh"

class BroadCastSocketManeger: public ISocketManager {
public:
  BroadCastSocketManeger(int nodeIndex,
      char* _initMulticastHost, int _initMultiCastPort, int _clusterSize);
  void initConnections();
  int recvFromNode(int, char*, int);
  void sendToNode(int, char*, int);
  void setMasterSocketManager(MasterSocketManager*);

  void resetFinishCount();
  void finishedSocket(int socketIndex);
  bool isFinishedAll();

  Selector* getSelector(int timeout = 0);
  ~BroadCastSocketManeger();

private:
  void initSockets();
  void initBroadcaster();
  void initListeners();

  int nodeIndex;
  char initMulticastHost[1024];
  int initMultiCastPort;
  int clusterSize;
  bool isFinished;

  UDPMulticastPublisher* broadcastSocket;
  vector<SocketConnection*> listeners;
  MasterSocketManager* masterSocketManager;
  log4cpp::Category* logger;

};


#endif  // BROADCAST_SOCKET_MANAGER_H_
