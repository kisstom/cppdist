#ifndef MASTER_SOCKET_MANAGER_H_
#define MASTER_SOCKET_MANAGER_H_

#include <vector>

#include "socket.h"
#include "log4cpp/Category.hh"


class MasterSocketManager {
public:
  MasterSocketManager();
  void connectToMaster(char*, int);
  int recvFromMaster(int, char*);
  void sendFailToMaster();
  void sendReadyToMaster();
  void sendEmptyToMaster();
  void setIp();
  void initSockets();
private:
  SocketConnection* master_socket_;
  char ip_[1024];
  int slave_port_;
  log4cpp::Category* logger_;
};


#endif  // MASTER_SOCKET_MANAGER_H_
