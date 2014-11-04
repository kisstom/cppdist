#ifndef ZMQ_SOCKET_H_
#define ZMQ_SOCKET_H_

#include "../isocket/isocket.h"
#include "zmq.hpp"
#include "log4cpp/Category.hh"

class ZMQSocket : public ISocket {
public:
  ZMQSocket(zmq::context_t* context);

  // virtual functions

  bool IsClosed();
  void Close();
  int GetPort();
  int Send(int length, const char *buf);
  int Recv(int limit, char* buf);

  //

  void bind(char* host);
  void connect(char* host);

  virtual ~ZMQSocket();
private:
  zmq::context_t* context;
  zmq::socket_t* socket;
  bool isClosed;
  log4cpp::Category* logger;

};

#endif  // ZMQ_SOCKET_H_
