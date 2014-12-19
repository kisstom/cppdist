#ifndef SOCKET_MANAGER_FACTORY_H_
#define SOCKET_MANAGER_FACTORY_H_

#include "../../../common/components/socket/isocket/isocket_manager.h"
#include <tr1/unordered_map>
#include <string>

using std::tr1::unordered_map;
using std::string;

class SocketManagerFactory {
public:
  ISocketManager* createSocketManager(unordered_map<string, string>* params);
};


#endif  // SOCKET_MANAGER_FACTORY_H_
