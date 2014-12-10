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
}

ClientSocketManager::~ClientSocketManager () {
  for (int i = 0; i < (int) listenerSockets.size(); ++i) {
    if (listenerSockets[i])
      delete listenerSockets[i];
  }
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
  logger->info("Publishing on port %d.", clusterConfig->getPort(selfIndex));
  sprintf(ip, "tcp://*:%d", clusterConfig->getPort(selfIndex));
  publisherSocket->bind(ip);
}

void ClientSocketManager::setClusterConfig(ClusterConfig* _clusterConfig) {
  clusterConfig = _clusterConfig;
}

void ClientSocketManager::initSubscribes() {
  listenerSockets.resize(numCluster);

  char ip[1024];
  for (int i = 0; i < numCluster; ++i) {
    if (i == selfIndex) {
      listenerSockets[i] = NULL;
      continue;
    }

    logger->info("Listening on host %s and on port %d", clusterConfig->getHost(i), clusterConfig->getPort(i));
    sprintf(ip, "tcp://%s:%d", clusterConfig->getHost(i), clusterConfig->getPort(i));
    listenerSockets[i] = new zmq::socket_t(*context, ZMQ_SUB);
    listenerSockets[i]->connect(ip);
    listenerSockets[i]->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  }

  pollItems = new zmq::pollitem_t [numCluster - 1];
  buildPoll();
}

int ClientSocketManager::findFdIsSet() {
  int zevents = 0;
  size_t zevents_len = sizeof(zevents);

  int fd;
  size_t sint = sizeof(fd);
  for (int i = 0; i < numCluster; ++i) {
    if (i == selfIndex) continue;

    listenerSockets[i]->getsockopt(ZMQ_FD, &fd, &sint);
    if (FD_ISSET(fd, &fdSet)) {
      return i;
    }
  }

  return -1;
}

int ClientSocketManager::buildFDSet() {
  FD_ZERO(&fdSet);

  int maxFd = -1;
  int fd;
  size_t sint = sizeof(fd);
  for (int i = 0; i < numCluster; ++i) {
    if (i == selfIndex) continue;

    listenerSockets[i]->getsockopt(ZMQ_FD, &fd, &sint);
    logger->info("file descriptor: %d", fd);
    FD_SET(fd, &fdSet);
    if (fd > maxFd) {
      maxFd = fd;
    }
  }

  return maxFd;
}

void ClientSocketManager::buildPoll() {
  int actIndex;
  for (int i = 0; i < numCluster; ++i) {
    if (i == selfIndex) continue;
    if (i > selfIndex) {
      actIndex = i - 1;
    } else {
      actIndex = i;
    }

    pollItems[actIndex] = {*listenerSockets[i], 0, ZMQ_POLLIN, 0};
  }

}

void ClientSocketManager::destroyPoll() {
}

void ClientSocketManager::publishEndSignal() {
  logger->info("Publishing finish %d", selfIndex);
  zmq::message_t message(2);
  char msg[2] = "0";
  strcpy((char*) message.data(), msg);
  publisherSocket->send(message);
}

void ClientSocketManager::readFromPoll() {
  zmq::message_t m(1024);
  int actIndex;
  bool found = false;
  for (int i = 0; i < numCluster; ++i) {
    if (i == selfIndex) continue;
    if (i > selfIndex) {
      actIndex = i - 1;
    } else {
      actIndex = i;
    }

    if (pollItems[actIndex].revents & ZMQ_POLLIN) {
      listenerSockets[i]->recv(&m);
      logger->info("Received from poll %s.", (char*) m.data());
      found = true;
      break;
    }
  }

  if (!found) logger->info("Could not read from poll.");
}

void ClientSocketManager::run() {
  logger->info("Starting run.");

  while (1) {
    zmq::poll (pollItems, numCluster - 1, -1);
    readFromPoll();
    incrementFinishCounter();
    if (isFinished()) break;
  }

  logger->info("Finished ClientSocketManager.");
}

void ClientSocketManager::resetFinishCounter() {
  mutex.lock();
  finishCounter = 0;
  logger->info("Resetting finish counter to %d", finishCounter);
  mutex.unlock();
}

bool ClientSocketManager::isFinished() {
  bool isFinished;

  mutex.lock();
  isFinished = numCluster - 1 == finishCounter;
  mutex.unlock();

  return isFinished;
}

void ClientSocketManager::incrementFinishCounter() {
  mutex.lock();
  ++finishCounter;
  mutex.unlock();
}

void ClientSocketManager::setMasterSocketManager(MasterSocketManager* manager) {
  masterSocketManager = manager;
}
