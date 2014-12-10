/*
 * socket_manager.cpp
 *
 *  Created on: 2013.08.12.
 *      Author: kisstom
 */

#include "socket_manager.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

SocketManager::SocketManager() {
  logger_ = NULL;
  master_socket_ = NULL;
  self_socket_ = NULL;
  masterSocketManager = NULL;
  slave_port_ = -1;
  clusterSize = -1;
  expectedFinish = -1;
  numFinished = -1;
}

SocketManager::SocketManager (int _clusterSize, int port) {
  logger_ = &log4cpp::Category::getInstance(std::string("SocketManager"));
  master_socket_ = NULL;
  self_socket_ = NULL;
  masterSocketManager = NULL;
  slave_port_ = port;
  clusterSize = _clusterSize;
  expectedFinish = clusterSize - 1;
  numFinished = -1;
}

SocketManager::~SocketManager() {
	if (self_socket_) delete self_socket_;
	for (int i = 0; i < (int) sender_sockets_.size(); ++i) {
    if (sender_sockets_[i]) delete sender_sockets_[i];
	}

	for (int i = 0; i < (int) receiver_sockets_.size(); ++i) {
    if (receiver_sockets_[i]) delete receiver_sockets_[i];
	}
}

void SocketManager::setMasterSocketManager(MasterSocketManager* manager) {
  masterSocketManager = manager;
}

void SocketManager::initClient() {
	logger_->info("Creating client at port %d", slave_port_);
  self_socket_ = ServerSocket::Create(slave_port_);
  logger_->info("Created client at port %d.", slave_port_);
}


void SocketManager::initConnections()
{
  logger_->info("Initing connections.");
  initClient();
  initSockets();

  char buf[1024], instr[1024], host[1024];
  int slave_index, port;
  int num_round = receiver_sockets_.size() % 2 ? receiver_sockets_.size() :
    receiver_sockets_.size() - 1;
  for (int i = 0; i < num_round; ++i)
  {
    masterSocketManager->recvFromMaster(1024, buf);
    if (strcmp(buf, "die") == 0) throw MasterException();
    if (strcmp(buf, "relax") == 0) {
      logger_->info("Connection info: relax.");
      masterSocketManager->sendReadyToMaster();
      continue;
    }
    if (sscanf(buf, "%s %d %s %d", instr, &slave_index, host, &port) == EOF) {
      throw RecvError(slave_port_);
    }
    logger_->info("Connection info: %s %d %s %d", instr, slave_index, host, port);
    if (!strcmp(instr, "Accept")) {
      receiver_sockets_[slave_index] = self_socket_->Accept();
      sender_sockets_[slave_index] = SocketConnection::Connect(string(host), port);
    }
    else if (!strcmp(instr, "Connect"))
    {
      sender_sockets_[slave_index] = SocketConnection::Connect(string(host), port);
      receiver_sockets_[slave_index] = self_socket_->Accept();
    }
    masterSocketManager->sendReadyToMaster();
  }
  for (int i = 0; i < (int) sender_sockets_.size(); ++i) {
    logger_->info("My port %d connection %d pointer %d",  slave_port_, i, NULL == receiver_sockets_[i]);
  }

  logger_->info("Finished connections.");
}


int SocketManager::recvFromNode(int limit, char* buf, int nodeIndex) {
	return receiver_sockets_[nodeIndex]->Recv(limit, buf);
}

void SocketManager::sendToNode(int limit, char* buf, int nodeIndex) {
	sender_sockets_[nodeIndex]->Send(limit, buf);
}

void SocketManager::initSockets() {
	logger_->info("Initing sockets size %d.", clusterSize);
	receiver_sockets_.resize(clusterSize, NULL);
	sender_sockets_.resize(clusterSize, NULL);
}

Selector* SocketManager::getSelector(int timeout = 0) {
  Selector* selector = new Selector(timeout);
  selector->Init(&receiver_sockets_);
  return selector;
}


void SocketManager::resetFinishCount() {
  numFinished = 0;
}

void SocketManager::finishedSocket(int socketIndex) {
  ++numFinished;
}

bool SocketManager::isFinishedAll() {
  return numFinished == expectedFinish;
}



