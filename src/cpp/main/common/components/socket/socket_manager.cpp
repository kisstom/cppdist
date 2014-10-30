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

SocketManager::SocketManager () {
  logger_ = &log4cpp::Category::getInstance(std::string("SocketManager"));
  master_socket_ = NULL;
  self_socket_ = NULL;
}

SocketManager::~SocketManager() {
	if (master_socket_) delete master_socket_;
	if (self_socket_) delete self_socket_;
	for (int i = 0; i < (int) sender_sockets_.size(); ++i) {
    if (sender_sockets_[i]) delete sender_sockets_[i];
	}

	for (int i = 0; i < (int) receiver_sockets_.size(); ++i) {
    if (receiver_sockets_[i]) delete receiver_sockets_[i];
	}
}

void SocketManager::initClient(int slave_port) {
	logger_->info("Creating client at port %d", slave_port);
  self_socket_ = ServerSocket::Create(slave_port);
  slave_port_ = slave_port;
  logger_->info("Created client at port %d.", slave_port_);
}

void SocketManager::connectToMaster(char* master_host, int master_port) {
	logger_->info("Connecting to master on host %s and port %d", master_host, master_port);
	setIp();
  master_socket_ = SocketConnection::Connect(string(master_host), master_port);
  char init_instr[1024];
  sprintf(init_instr, "%d,%s", slave_port_, ip_);
  master_socket_->Send(strlen(init_instr)+1, init_instr);
}

void SocketManager::setIp() {
    static struct ifreq ifreqs[32];
    struct ifconf ifconf;
    memset(&ifconf, 0, sizeof(ifconf));
    ifconf.ifc_req = ifreqs;
    ifconf.ifc_len = sizeof(ifreqs);

    int sd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sd >= 0);

    int r = ioctl(sd, SIOCGIFCONF, (char *)&ifconf);
    assert(r == 0);

    //bool found = false;
    for(uint i = 0; i < ifconf.ifc_len/sizeof(struct ifreq); ++i)
    {
        if (strstr(ifreqs[i].ifr_name, "eth") != NULL) {
          strcpy(ip_, inet_ntoa(((struct sockaddr_in *)&ifreqs[i].ifr_addr)->sin_addr));
          //found = true;
          break;
        }
    }

    close(sd);
}

void SocketManager::sendReadyToMaster(){
	//logger_->info("Sending ready to master.");
  master_socket_->Send(6, "ready");
}

void SocketManager::sendEmptyToMaster(){
	logger_->info("Sending empty to master.");
  master_socket_->Send(6, "empty");
}

void SocketManager::sendFailToMaster(){
	logger_->info("Sending fail to master.");
  master_socket_->Send(5, "fail");
}

void SocketManager::initConnections()
{
  logger_->info("Initing connections.");
  char buf[1024], instr[1024], host[1024];
  int slave_index, port;
  int num_round = receiver_sockets_.size() % 2 ? receiver_sockets_.size() :
    receiver_sockets_.size() - 1;
  for (int i = 0; i < num_round; ++i)
  {
    master_socket_->Recv(1024, buf);
    if (strcmp(buf, "die") == 0) throw MasterException();
    if (strcmp(buf, "relax") == 0) {
      logger_->info("Connection info: relax.");
      sendReadyToMaster();
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
    sendReadyToMaster();
  }
  for (int i = 0; i < (int) sender_sockets_.size(); ++i) {
    logger_->info("My port %d connection %d pointer %d",  slave_port_, i, NULL == receiver_sockets_[i]);
  }

  logger_->info("Finished connections.");
}

int SocketManager::recvFromMaster(int limit, char* buf) {
	master_socket_->Recv(limit, buf);
}

int SocketManager::recvFromNode(int limit, char* buf, int nodeIndex) {
	return receiver_sockets_[nodeIndex]->Recv(limit, buf);
}

void SocketManager::sendToNode(int limit, char* buf, int nodeIndex) {
	sender_sockets_[nodeIndex]->Send(limit, buf);
}

void SocketManager::initSockets(int socket_size) {
	logger_->info("Initing sockets size %d.", socket_size);
	receiver_sockets_.resize(socket_size, NULL);
	sender_sockets_.resize(socket_size, NULL);
}

vector<SocketConnection*> SocketManager::getReceiverSockets() {
	return receiver_sockets_;
}

vector<SocketConnection*> SocketManager::getSenderSockets() {
  return sender_sockets_;
}
