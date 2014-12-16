#ifndef MOCK_MASTER_SOCKET_MANAGER_H_
#define MOCK_MASTER_SOCKET_MANAGER_H_


#include "gmock/gmock.h"
#include "../../main/common/components/socket/master_socket_manager.h"

class MockMasterSocketManager : public MasterSocketManager {
public:
  MOCK_METHOD2(connectToMaster, void(char*, int));
  MOCK_METHOD2(recvFromMaster, int(int, char*));
  MOCK_METHOD0(sendReadyToMaster, void());
};


#endif  // MOCK_MASTER_SOCKET_MANAGER_H_
