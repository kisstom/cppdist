#include "./multicast_socket_manager.h"
#include <cstring>
#include <cmath>
#include <cstdio>

MulticastSocketManager::MulticastSocketManager(int _nodeIndex, int _startingHash,
      char* _initMulticastHost, int _initMultiCastPort, int _clusterSize) :
      ipIndexMaker(_nodeIndex, _clusterSize) {
  nodeIndex = _nodeIndex;
  startingHash = _startingHash;
  strcpy(initMulticastHost, _initMulticastHost);
  initMultiCastPort = _initMultiCastPort;
  clusterSize = _clusterSize;
  masterSocketManager = NULL;
}

int MulticastSocketManager::recvFromNode(int limit, char* buffer, int socketIndex) {
  return listeners[socketIndex]->recv(limit, buffer);
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
  int ipIndex, add;

  char actHost[1024];
  for (int pi = 1; pi <= publisherSize; ++pi) {
    ipIndex = ipIndexMaker.createPublisherIPIndex(pi);

    add = ipIndex + startingHash;
    publishers[pi - 1] = new UDPMulticastPublisher;

    sprintf(actHost, "%s%d", initMulticastHost, add);
    publishers[pi - 1]->create(actHost, add);
  }

}

void MulticastSocketManager::initListeners() {
  int listenerSize = pow(2, clusterSize - 1);

  if (listenerSize < 1) return;
  int ipIndex, port;

  char actHost[1024];
  for (int pi = 0; pi < listenerSize; ++pi) {
    ipIndex = ipIndexMaker.createClientIPIndex(pi);

    port = ipIndex + startingHash;
    listeners[pi] = new UDPMulticastReceiver;

    sprintf(actHost, "%s%d", initMulticastHost, port);
    listeners[pi]->connectToMulticastIp(actHost, port);
  }

}

void MulticastSocketManager::initSockets() {
  int publisherSize = pow(2, clusterSize - 1) - 1;
  if (publisherSize < 1) return;

  publishers.resize(publisherSize, NULL);

  int listenerSize = pow(2, clusterSize - 1);
  if (listenerSize < 1) return;

  listeners.resize(listenerSize, NULL);
}

