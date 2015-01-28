#include "broadcast_socket_manager.h"

BroadCastSocketManeger::BroadCastSocketManeger(int _nodeIndex,
    char* _initMulticastHost, int _initMultiCastPort, int _clusterSize) {
  logger = &log4cpp::Category::getInstance(std::string("BroadCastSocketManeger"));
  nodeIndex = _nodeIndex;
  strcpy(initMulticastHost, _initMulticastHost);
  initMultiCastPort = _initMultiCastPort;
  clusterSize = _clusterSize;
  masterSocketManager = NULL;
  broadcastSocket = NULL;
  isFinished = true;
  logger->info("Constructor called.");
}

BroadCastSocketManeger::~BroadCastSocketManeger() {
  for (int i = 0; i < (int) listeners.size(); ++i) {
    if (listeners[i])
      delete listeners[i];
  }

  if (broadcastSocket) delete broadcastSocket;
}

void BroadCastSocketManeger::initConnections() {
  logger->info("Initing connections");
  initSockets();
  initBroadcaster();

  int limit = 1024;
  char msg[1024];
  masterSocketManager->sendReadyToMaster();
  masterSocketManager->recvFromMaster(limit, msg);

  initListeners();
}

int BroadCastSocketManeger::recvFromNode(int limit, char* buffer, int socketIndex) {
  return listeners[socketIndex]->Recv(limit, buffer);
}

void BroadCastSocketManeger::
sendToNode(int limit, char* buffer, int socketIndex) {
  broadcastSocket->send(limit, buffer);
}

void BroadCastSocketManeger::
setMasterSocketManager(MasterSocketManager*_masterSocketManager) {
  masterSocketManager = _masterSocketManager;
}

void BroadCastSocketManeger::resetFinishCount() {
  isFinished = false;
}

void BroadCastSocketManeger::finishedSocket(int socketIndex) {
  isFinished = true;
}

bool BroadCastSocketManeger::isFinishedAll() {
  return isFinished;
}

Selector* BroadCastSocketManeger::getSelector(int timeout) {
  Selector* selector = new Selector(timeout);
  selector->Init(&listeners);
  return selector;
}

void BroadCastSocketManeger::initSockets() {
  listeners.resize(clusterSize - 1, NULL);
}

void BroadCastSocketManeger::initBroadcaster() {
  int broadCastPort = initMultiCastPort + nodeIndex;
  char broadCastHost[1024];
  sprintf(broadCastHost, "%s%d", initMulticastHost, nodeIndex);
  broadcastSocket = new UDPMulticastPublisher;
  broadcastSocket->create(broadCastHost, broadCastPort);
}

void BroadCastSocketManeger::initListeners() {
  UDPMulticastReceiver* listener = NULL;
  char actHost[1024];
  int port;

  for (int listenerIndex = 0; listenerIndex < clusterSize; ++listenerIndex) {
    if (listenerIndex == nodeIndex) continue;

    sprintf(actHost, "%s%d", initMulticastHost, listenerIndex);
    port = listenerIndex + initMultiCastPort;
    listener->connectToMulticastIp(actHost, port);

    if (listenerIndex < nodeIndex) {
      listeners[listenerIndex] = listener;
    } else {
      listeners[listenerIndex - 1] = listener;
    }
  }
}






