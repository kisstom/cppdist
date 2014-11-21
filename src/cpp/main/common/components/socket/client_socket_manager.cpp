#include "client_socket_manager.h"
#include <cstring>

ClientSocketManager::ClientSocketManager(
    int _selfIndex, int _numCluster) {
  context = new zmq::context_t(1);
  logger = &log4cpp::Category::getInstance(std::string("ClientSocketManager"));
  publisherSocket = new zmq::socket_t(*context, ZMQ_PUB);

  selfIndex = _selfIndex;
  numCluster = _numCluster;

  finishCounter = 0;
  clusterConfig = NULL;
  sendHWM = 1024;

  listenerSocket = new zmq::socket_t(*context, ZMQ_SUB);
  listenerSocket->setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

ClientSocketManager::~ClientSocketManager () {
  delete listenerSocket;
  delete publisherSocket;
}

bool ClientSocketManager::setUp() {
  char msg[1024];
  masterSocketManager->recvFromMaster(1024, msg);
  initPublisher();
  masterSocketManager->sendReadyToMaster();

  masterSocketManager->recvFromMaster(1024, msg);
  initSubscribes();
  masterSocketManager->sendReadyToMaster();
  return true;
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
  logger->info("Publishing finish %d", selfIndex);
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

  logger->info("ClientSocketManager finished.");
}

void ClientSocketManager::resetFinishCounter() {
  finishCounter = 0;
}

bool ClientSocketManager::isFinished() {
  logger->info("Is finished counter %d cluster size %d", finishCounter, numCluster);
  return numCluster - 1 == finishCounter;
}

void ClientSocketManager::incrementFinishCounter() {
  logger->info("Incrementing counter %d at node %d", finishCounter, selfIndex);
  ++finishCounter;
}

void ClientSocketManager::setMasterSocketManager(MasterSocketManager* manager) {
  masterSocketManager = manager;
}
