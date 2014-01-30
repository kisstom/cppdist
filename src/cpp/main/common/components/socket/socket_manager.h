/*
 * socket_manager.h
 *
 *  Created on: 2013.08.12.
 *      Author: kisstom
 */

#ifndef SOCKET_MANAGER_H_
#define SOCKET_MANAGER_H_

#include "socket.h"
#include "log4cpp/Category.hh"

class SocketManager {
public:
	SocketManager();
	virtual void initSockets(int);
  virtual void initClient(int);
  virtual void connectToMaster(char*, int);
  virtual int recvFromMaster(int, char*);
  virtual void sendFailToMaster();
  virtual void sendReadyToMaster();
  virtual void sendEmptyToMaster();
  virtual void initConnections();
  virtual int recvFromNode(int, char*, int);
  virtual void sendToNode(int, char*, int);
  virtual void setIp();
  virtual vector<SocketConnection*> getReceiverSockets();
  virtual ~SocketManager();
private:
  ServerSocket* self_socket_;
  SocketConnection* master_socket_;
  vector<SocketConnection*> sender_sockets_;
  vector<SocketConnection*> receiver_sockets_;
  char ip_[1024];
  int slave_port_;
  log4cpp::Category* logger_;
};


#endif /* SOCKET_MANAGER_H_ */
