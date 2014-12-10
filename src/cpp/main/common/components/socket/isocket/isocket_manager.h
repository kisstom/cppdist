#ifndef ISOCKET_MANAGER_H_
#define ISOCKET_MANAGER_H_

#include "../master_socket_manager.h"

class ISocketManager {
public:
  virtual void initConnections() = 0;
  virtual int recvFromNode(int, char*, int) = 0;
  virtual void sendToNode(int, char*, int) = 0;
  virtual void setMasterSocketManager(MasterSocketManager*) = 0;

  virtual void resetFinishCount() = 0;
  virtual void finishedSocket(int socketIndex) = 0;
  virtual bool isFinishedAll() = 0;

  virtual Selector* getSelector(int timeout = 0) = 0;
  virtual ~ISocketManager() {}
};


#endif  // ISOCKET_MANAGER_H_
