#ifndef SLAVE_H_
#define SLAVE_H_

struct Slave {
  int port;
  char ip[1024];
  SocketConnection* socket;
  long minNode;
  long numNode;
};

#endif  // SLAVE_H_
