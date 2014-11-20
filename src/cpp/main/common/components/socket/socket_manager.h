/*
 * socket_manager.h
 *
 *  Created on: 2013.08.12.
 *      Author: kisstom
 */

#ifndef SOCKET_MANAGER_H_
#define SOCKET_MANAGER_H_

#include <gtest/gtest_prod.h>
#include "socket.h"
#include "master_socket_manager.h"
#include "log4cpp/Category.hh"

class SocketManager {
public:
	SocketManager();
	virtual void initSockets(int);
  virtual void initClient(int);
  virtual void initConnections();
  virtual int recvFromNode(int, char*, int);
  virtual void sendToNode(int, char*, int);

  void setMasterSocketManager(MasterSocketManager* manager);
  virtual vector<SocketConnection*> getReceiverSockets();
  virtual ~SocketManager();
private:
  MasterSocketManager* masterSocketManager;
  ServerSocket* self_socket_;
  SocketConnection* master_socket_;
  vector<SocketConnection*> sender_sockets_;
  vector<SocketConnection*> receiver_sockets_;
  char ip_[1024];
  int slave_port_;
  log4cpp::Category* logger_;

  FRIEND_TEST(SimpleMockTestSetup, testSetup);
  friend class SimpleMockTestSetup;
};


#endif /* SOCKET_MANAGER_H_ */
