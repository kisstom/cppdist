#ifndef CONNECTIONERROR_H
#define CONNECTIONERROR_H

#include <iostream>
#include <string.h>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

class ConnectionError: public exception {
  public:
    ConnectionError() {};
    virtual const char* what();
  protected:
    void getIp(char *ip);
    char error_msg_[1024];
};

class MasterException: public ConnectionError {
  public:
    MasterException() : ConnectionError() {}
};

class NodeFailException: public ConnectionError {
  public:
    NodeFailException(string msg) : ConnectionError() {
      strcpy(error_msg_, msg.c_str());
    }
};

class IOError: public ConnectionError {
  public:
    IOError(string msg) : ConnectionError() {
      strcpy(error_msg_, msg.c_str());
    }
};

class LogError: public ConnectionError {
  public:
    LogError(string msg) : ConnectionError() {
      strcpy(error_msg_, msg.c_str());
    }
};

class SelectError: public ConnectionError {
  public:
    SelectError();
};

class SocketError: public ConnectionError {
  public:
    SocketError(int port);
  protected:
    int port_;
};

class ServerCreateError: public SocketError {
  public:
    ServerCreateError(int port);
};

class AcceptError: public SocketError {
  public:
    AcceptError(int port);
};

class BindError: public SocketError {
  public:
    BindError(int port);
};

class RecvError: public SocketError {
  public:
    RecvError(int port);
};

class ConnectError: public SocketError {
  public:
    ConnectError(string host, int port);
    ~ConnectError () throw () {}
  protected:
    // TODO megerteni
    string host_;
};

#endif 
