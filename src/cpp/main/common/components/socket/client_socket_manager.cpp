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

  //pollItems.resize(numCluster - 1, NULL);
  //pollItems = new zmq_pollitem_t[numCluster - 1];
  //buildFDSet();
}

int ClientSocketManager::findFdIsSet() {
  int zevents = 0;
  size_t zevents_len = sizeof(zevents);

  int fd;
  size_t sint = sizeof(fd);
  for (int i = 0; i < numCluster; ++i) {
    if (i == selfIndex) continue;

    //listenerSockets[i]->getsockopt(ZMQ_EVENTS, &zevents, &zevents_len);
    listenerSockets[i]->getsockopt(ZMQ_FD, &fd, &sint);
    if (FD_ISSET(fd, &fdSet)) {

    //if (zevents & ZMQ_POLLIN) {
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
  /*for (int i = 0; i < numCluster; ++i) {
    if (i == selfIndex) continue;
    if (i > selfIndex) {
      actIndex = i - 1;
    } else {
      actIndex = i;
    }

    //pollItems[actIndex] = new zmq::pollitem_t;
    logger->info("Socketindex %d", i);
    pollItems[actIndex].socket = listenerSockets[i];
    if (listenerSockets[i] == NULL) logger->info("Hat akkor mi ez?");
    pollItems[actIndex].fd = 0;
    pollItems[actIndex].events = ZMQ_POLLIN;
    pollItems[actIndex].revents = 0;
  }*/

}

void ClientSocketManager::destroyPoll() {
  /*for (int i = 0; i < (int) pollItems.size(); ++i) {
    delete pollItems[i];
  }*/
}

void ClientSocketManager::publishEndSignal() {
  //sleep(2);
  logger->info("Publishing finish %d", selfIndex);
  zmq::message_t message(2);
  char msg[2] = "0";
  strcpy((char*) message.data(), msg);
  publisherSocket->send(message);
}

void ClientSocketManager::run() {
  logger->info("Starting run.");
  zmq::message_t m(1024);
  resetFinishCounter();

  int pollIndex;
  if (selfIndex == 0) {
    pollIndex = 1;
  } else {
    pollIndex = 0;
  }

  zmq::pollitem_t pollItems[] = {
            {*listenerSockets[pollIndex], 0, ZMQ_POLLIN, 0}
        };

  while (1) {
    //buildPoll();

    logger->info("In the loop waiting for finish.");
    zmq::poll (&pollItems[0], numCluster - 1, -1);

    //logger->info("%d is set for reading.", si);
    //listenerSockets[si]->recv(&m);
    //logger->info("Received: %s.", (char*) m.data());
    //listenerSocket->recv(&m);
    // Check whether it is really 0
    incrementFinishCounter();
    if (isFinished()) break;
  }

  //destroyPoll();

  logger->info("ClientSocketManager finished.");
}

void ClientSocketManager::resetFinishCounter() {
  finishCounter = 0;
  logger->info("Resetting finish counter to %d", finishCounter);
}

bool ClientSocketManager::isFinished() {
  //logger->info("Is finished counter %d cluster size %d", finishCounter, numCluster);
  return numCluster - 1 == finishCounter;
}

void ClientSocketManager::incrementFinishCounter() {
  logger->info("Incrementing counter %d at node %d", finishCounter, selfIndex);
  ++finishCounter;
}

void ClientSocketManager::setMasterSocketManager(MasterSocketManager* manager) {
  masterSocketManager = manager;
}
