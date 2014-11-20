#include "client_socket_manager.h"
#include <cstring>

ClientSocketManager::ClientSocketManager(
    int _selfIndex, int _numCluster, int _startClientPort) {
  context = new zmq::context_t(1);
  logger = &log4cpp::Category::getInstance(std::string("ClientSocketManager"));
  publisherSocket = new zmq::socket_t(*context, ZMQ_PUB);

  selfIndex = _selfIndex;
  numCluster = _numCluster;
  startClienPort = _startClientPort;

  finishCounter = 0;
  clusterConfig = NULL;
  sendHWM = 1024;

  listenerSocket = new zmq::socket_t(*context, ZMQ_SUB);
  listenerSocket->setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

void ClientSocketManager::initPublisher() {
  char ip[1024];
  publisherSocket->setsockopt(ZMQ_SNDHWM, &sendHWM, sizeof(sendHWM));
  sprintf(ip, "tcp://*:%d", clusterConfig->getPort(selfIndex));
  publisherSocket->bind(ip);
}

void ClientSocketManager::setClusterConfig(ClusterConfig* _clusterConfig) {
  clusterConfig = _clusterConfig;
}

void ClientSocketManager::initSubscribes() {
  char ip[1024];
  for (int i = 0; i < numCluster; ++i) {
    if (i == selfIndex) continue;
    sprintf(ip, "tcp://%s:%d", clusterConfig->getHost(i), clusterConfig->getPort(i));
    listenerSocket->connect(ip);
  }
}

void ClientSocketManager::publishEndSignal() {
  zmq::message_t message(1);
  char msg[2] = "0";
  strcpy((char*) message.data(), msg);
  publisherSocket->send(message);
}

void ClientSocketManager::run() {
  zmq::message_t m(20);
  while (!isFinished()) {
    listenerSocket->recv(&m);
    // Check whether it is really 0
    incrementFinishCounter();
  }
}

void ClientSocketManager::resetFinishCounter() {
  finishCounter = 0;
}

bool ClientSocketManager::isFinished() {
  return numCluster == finishCounter;
}

void ClientSocketManager::incrementFinishCounter() {
  ++finishCounter;
}
