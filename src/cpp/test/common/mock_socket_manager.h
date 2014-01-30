/*
 * mock_socket_manager.h
 *
 *  Created on: 2013.08.13.
 *      Author: kisstom
 */

#ifndef MOCK_SOCKET_MANAGER_H_
#define MOCK_SOCKET_MANAGER_H_

#include "gmock/gmock.h"
#include "../../main/common/components/socket/socket.h"
#include "../../main/common/components/socket/socket_manager.h"

class MockSocketManager : public SocketManager {
public:
	MOCK_METHOD1(initSockets, void(int));
	MOCK_METHOD1(initClient, void(int));
	MOCK_METHOD2(connectToMaster, void(char*, int));
	MOCK_METHOD2(recvFromMaster, int(int, char*));
	MOCK_METHOD0(sendFailToMaster, void());
	MOCK_METHOD0(sendReadyToMaster, void());
	MOCK_METHOD0(sendEmptyToMaster, void());
	MOCK_METHOD0(initConnections, void());
	MOCK_METHOD0(setIp, void());
	MOCK_METHOD3(recvFromNode, int(int, char*, int));
	MOCK_METHOD3(sendToNode, void(int, char*, int));
	MOCK_METHOD0(getReceiverSockets, vector<SocketConnection*>());
	//~MockSocketManager() {printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFfff");}
};


#endif /* MOCK_SOCKET_MANAGER_H_ */
