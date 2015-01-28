#include <cstdlib>

#include "socket_manager_factory.h"
#include "../../../common/components/socket/socket_manager.h"
#include "../../../common/components/socket/multicast_socket_manager.h"
#include "../../../common/components/socket/broadcast_socket_manager.h"
#include <exception>

ISocketManager* SocketManagerFactory::createSocketManager(unordered_map<string, string>* params) {
  ISocketManager* socketManager = NULL;

  int initSlavePort = atoi((*params)["INIT_SLAVE_PORT"].c_str());
  int slaveIndex = atoi((*params)["SLAVE_INDEX"].c_str());
  int numSlaves = atoi((*params)["NUM_SLAVES"].c_str());
  char multicastIpPrefix[1024];
  strcpy(multicastIpPrefix, "225.0.0.");

  if (params->find("MULTI") != params->end()) {
    if ((*params)["MULTI"].compare("MULTICAST") == 0) {
      socketManager = new MulticastSocketManager(slaveIndex, 1000,
          multicastIpPrefix, initSlavePort, numSlaves);
    } else if ((*params)["MULTI"].compare("BROADCAST") == 0) {
      socketManager = new BroadCastSocketManeger(
          slaveIndex, multicastIpPrefix, initSlavePort, numSlaves);
    } else {
      throw std::exception();
    }
  } else {
    socketManager = new SocketManager(numSlaves, initSlavePort + slaveIndex);
  }

  return socketManager;
}
