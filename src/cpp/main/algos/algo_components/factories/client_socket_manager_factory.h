#ifndef CLIENT_SOCKET_MANAGER_FACTORY_H_
#define CLIENT_SOCKET_MANAGER_FACTORY_H_

#include "../../../common/components/socket/client_socket_manager.h"
#include "../../../common/components/socket/cluster_config.h"
#include "../../../common/util/util.h"
#include <tr1/unordered_map>
#include <vector>
#include <string>

using std::tr1::unordered_map;
using std::string;
using std::vector;


class ClientSocketManagerFactory {
public:
  ClientSocketManager* createClientSocketManager(unordered_map<string, string>* params,
      vector<std::pair<string, string> >* hostAndPort);
private:
  ClusterConfig* createClusterConfig(vector<std::pair<string, string> >* hostAndPort,
      int initSlavePort, int numSlaves);

  Util util_;
};


#endif  // CLIENT_SOCKET_MANAGER_FACTORY_H_
