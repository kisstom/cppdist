/*
 * Copyright 2011 SZTAKI
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <vector>

using std::string;
using std::vector;

#include <fcntl.h>
#include  "../../util/ConnectionError.h"
#include "log4cpp/Category.hh"

class Socket {
 protected:
  int socket_file_descriptor;
  bool isClosed_;
  int socketType;
  log4cpp::Category* logger;
 public:
  Socket();
  virtual ~Socket();
  bool IsClosed();
  bool isConnected();
  int GetFileDescriptor();
  void Close();
};

class SocketConnection;

class ServerSocket : public Socket{
 private:

  int port_;
  ServerSocket();
 public:
  static ServerSocket *Create(int port);
  ~ServerSocket();
  SocketConnection *Accept();
  int GetPort();
};

class SocketConnection : public Socket {
  friend class ServerSocket;
 private:

  int port_;
  string host_;
  SocketConnection();
 public:
  static SocketConnection *Connect(string host, int port);
  ~SocketConnection();
  int Send(int length, const char *buf);
  int SendCStr(const char *msg);
  int SendStr(const string &msg);
  int Recv(int limit, char* buf);
  int GetPort();
  string GetHost();
};

class Selector {
 private:
  log4cpp::Category* logger;
  vector<SocketConnection *> sockets_;
  fd_set socks_;
  int max_fd_;
  void BuilFdSet();
  int RandStart();
 public:
  Selector();
  void Init(vector<SocketConnection *> connections);
  SocketConnection *Select();
  int SelectIndex();
};

#endif

