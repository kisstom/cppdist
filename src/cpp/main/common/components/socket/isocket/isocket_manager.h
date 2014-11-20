#ifndef ISOCKET_MANAGER_H_
#define ISOCKET_MANAGER_H_

#include "../master_socket_manager.h"

class ISocketManager {
public:
  virtual void initSockets() = 0;
  virtual void initConnections() = 0;
  virtual int recvFromNode(int, char*, int) = 0;
  virtual void sendToNode(int, char*, int) = 0;
  virtual void setMasterSocketManager(MasterSocketManager*) = 0;
  virtual ~ISocketManager() {}
};


#endif  // ISOCKET_MANAGER_H_
