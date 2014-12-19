#include <cstdlib>

#include "socket_manager_factory.h"
#include "../../../common/components/socket/socket_manager.h"
#include "../../../common/components/socket/multicast_socket_manager.h"

ISocketManager* SocketManagerFactory::createSocketManager(unordered_map<string, string>* params) {
  ISocketManager* socketManager = NULL;

  int initSlavePort = atoi((*params)["INIT_SLAVE_PORT"].c_str());
  int slaveIndex = atoi((*params)["SLAVE_INDEX"].c_str());
  int numSlaves = atoi((*params)["NUM_SLAVES"].c_str());

  if (params->find("MULTI") != params->end()) {
    bool isMulticast = atoi((*params)["MULTI"].c_str());
    if (isMulticast) {
      socketManager = new MulticastSocketManager(slaveIndex, 1000,
          "225.0.0.", initSlavePort, numSlaves);
    } else {
      socketManager = new SocketManager(numSlaves, initSlavePort + slaveIndex);
    }
  } else {
    socketManager = new SocketManager(numSlaves, initSlavePort + slaveIndex);
  }

  return socketManager;
}
