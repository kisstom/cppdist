#ifndef UDPMULTICASTPUBLISHER_H_
#define UDPMULTICASTPUBLISHER_H_

#include "socket.h"

class UDPMulticastPublisher : public Socket {
public:
  int create(char* group, int port);
  int send(int limit, char* buffer);
private:
  struct sockaddr_in addr;
};


#endif  // UDPMULTICASTPUBLISHER_H_
