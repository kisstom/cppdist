#ifndef MASTER_SOCKET_MANAGER_H_
#define MASTER_SOCKET_MANAGER_H_

#include <vector>

#include "socket.h"
#include "log4cpp/Category.hh"


class MasterSocketManager {
public:
  MasterSocketManager();
  virtual void connectToMaster(char*, int);
  virtual int recvFromMaster(int, char*);
  virtual void sendFailToMaster();
  virtual void sendReadyToMaster();
  virtual void sendEmptyToMaster();
  virtual void setPort(int);

  virtual ~MasterSocketManager();
private:
  void setIp();

  SocketConnection* master_socket_;
  char ip_[1024];
  int slave_port_;
  log4cpp::Category* logger_;
};


#endif  // MASTER_SOCKET_MANAGER_H_
