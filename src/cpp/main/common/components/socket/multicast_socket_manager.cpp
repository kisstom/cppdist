#include "./multicast_socket_manager.h"
#include <cstring>
#include <cmath>
#include <cstdio>

MulticastSocketManager::MulticastSocketManager(int _nodeIndex, int _startingHash,
      char* _initMulticastHost, int _initMultiCastPort, int _clusterSize) :
      ipIndexMaker(_nodeIndex, _clusterSize) {
  nodeIndex = _nodeIndex;
  //startingHash = _startingHash;
  strcpy(initMulticastHost, _initMulticastHost);
  initMultiCastPort = _initMultiCastPort;
  clusterSize = _clusterSize;
  masterSocketManager = NULL;
}

MulticastSocketManager::~MulticastSocketManager() {
  for (int i = 0; i < (int) listeners.size(); ++i) {
    if (listeners[i])
      delete listeners[i];
  }

  for (int i = 0; i < (int) publishers.size(); ++i) {
    if (publishers[i])
      delete publishers[i];
  }
}

int MulticastSocketManager::recvFromNode(int limit, char* buffer, int socketIndex) {
  return listeners[socketIndex]->Recv(limit, buffer);
}

void MulticastSocketManager::sendToNode(int limit, char* buffer, int socketIndex) {
  publishers[socketIndex]->send(limit, buffer);
}

void MulticastSocketManager::initConnections() {
  initPublishers();
  masterSocketManager->sendReadyToMaster();
  initListeners();
  masterSocketManager->sendReadyToMaster();
}

void MulticastSocketManager::setMasterSocketManager(
    MasterSocketManager*_masterSocketManager ) {
  masterSocketManager = _masterSocketManager;
}

void MulticastSocketManager::initPublishers() {
  int publisherSize = pow(2, clusterSize - 1) - 1;

  if (publisherSize < 1) return;
  int ipIndex, port;

  char actHost[1024];
  for (int pi = 1; pi <= publisherSize; ++pi) {
    ipIndex = ipIndexMaker.createPublisherIPIndex(pi);

    port = ipIndex + initMultiCastPort;
    publishers[pi - 1] = new UDPMulticastPublisher;

    sprintf(actHost, "%s%d", initMulticastHost, ipIndex);
    publishers[pi - 1]->create(actHost, port);
  }

}

void MulticastSocketManager::initListeners() {
  int listenerSize = pow(2, clusterSize - 1);

  if (listenerSize < 1) return;
  int ipIndex, port;

  UDPMulticastReceiver* listener;

  char actHost[1024];
  for (int pi = 0; pi < listenerSize; ++pi) {
    ipIndex = ipIndexMaker.createClientIPIndex(pi);

    port = ipIndex + initMultiCastPort;
    listener = new UDPMulticastReceiver;

    sprintf(actHost, "%s%d", initMulticastHost, ipIndex);
    listener->connectToMulticastIp(actHost, port);
    listeners[pi] = listener;
  }

}

void MulticastSocketManager::initSockets(int foo) {
  int publisherSize = pow(2, clusterSize - 1) - 1;
  if (publisherSize < 1) return;

  publishers.resize(publisherSize, NULL);

  int listenerSize = pow(2, clusterSize - 1);
  if (listenerSize < 1) return;

  listeners.resize(listenerSize, NULL);
}

Selector* MulticastSocketManager::getSelector() {
  Selector* selector = new Selector;
  selector->Init(&listeners);
  return selector;
}

