#include "./multicast_socket_manager.h"
#include <cstring>
#include <cmath>
#include <cstdio>

MulticastSocketManager::MulticastSocketManager(int _nodeIndex, int _startingHash,
      char* _initMulticastHost, int _initMultiCastPort, int _clusterSize) :
      ipIndexMaker(_nodeIndex, _clusterSize) {
  nodeIndex = _nodeIndex;
  strcpy(initMulticastHost, _initMulticastHost);
  initMultiCastPort = _initMultiCastPort;
  clusterSize = _clusterSize;
  masterSocketManager = NULL;
  logger = &log4cpp::Category::getInstance(std::string("MulticastSocketManager"));
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
  logger->info("Initing connections");
  initSockets();

  int limit = 1024;
  char msg[1024];

  masterSocketManager->recvFromMaster(limit, msg);
  initPublishers();
  masterSocketManager->sendReadyToMaster();
  masterSocketManager->recvFromMaster(limit, msg);
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
    logger->info("Create multicast %s %d index is %d", actHost, port, pi - 1);
    publishers[pi - 1]->create(actHost, port);
  }

}

void MulticastSocketManager::initListeners() {
  int listenerSize = pow(2, clusterSize - 1) - 1;

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

    // Also setting number of expected finishes.
    expectedFinish[pi] = getNumberOfZeros(ipIndex);
    logger->info("Connect to multicast %s %d index is %d, waiting for %d finishes ",
        actHost, port, pi, expectedFinish[pi]);
  }

}

void MulticastSocketManager::initSockets() {
  int publisherSize = pow(2, clusterSize - 1) - 1;
  if (publisherSize < 1) return;
  publishers.resize(publisherSize, NULL);

  int listenerSize = pow(2, clusterSize - 1) - 1;
  if (listenerSize < 1) return;
  listeners.resize(listenerSize, NULL);

  initExpectedFinishCounters();
}

void MulticastSocketManager::initExpectedFinishCounters() {
  //logger->info("Initing expected num of finished sockets %d with cluster size %d", clusterSize);
  expectedNumFinishedSockets = pow(2, clusterSize - 1) - 1;
  if (expectedNumFinishedSockets < 1) return;

  finishCounter.resize(expectedNumFinishedSockets, 0);
  expectedFinish.resize(expectedNumFinishedSockets, 0);
  finishedSockets = 0;
}

void MulticastSocketManager::resetFinishCount() {
  for (int i = 0; i < (int) finishCounter.size(); ++i) {
    finishCounter[i] = 0;
  }

  finishedSockets = 0;
}

void MulticastSocketManager::finishedSocket(int socketIndex) {
  ++finishCounter[socketIndex];
  logger->info("finish count for %d is %d expected %d",
      socketIndex, finishCounter[socketIndex], expectedFinish[socketIndex]);

  if (finishCounter[socketIndex] == expectedFinish[socketIndex]) ++finishedSockets;
  logger->info("Number of finished %d expected %d", finishedSockets, expectedNumFinishedSockets);
}

bool MulticastSocketManager::isFinishedAll() {
  return finishedSockets == expectedNumFinishedSockets;
}

Selector* MulticastSocketManager::getSelector(int timeout) {
  Selector* selector = new Selector(timeout);
  selector->Init(&listeners);
  return selector;
}

int MulticastSocketManager::getNumberOfZeros(int number) {
  int numOnes = 0;
  while (number) {
    numOnes += getLastBit(number);
    number >>= 1;
  }

  return clusterSize - numOnes;
}

int MulticastSocketManager::getLastBit(int number) {
  return number & 1;
}

