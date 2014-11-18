#ifndef UDPMULTICASTRECEIVER_H_
#define UDPMULTICASTRECEIVER_H_

#include "socket.h"

class UDPMulticastReceiver : public Socket {
public:
  void connectToMulticastIp(char* ip, int port);
  int recv(int limit, char* buffer);
private:
  struct sockaddr_in addr;
  int addrlen;
};



#endif  // UDPMULTICASTRECEIVER_H_
