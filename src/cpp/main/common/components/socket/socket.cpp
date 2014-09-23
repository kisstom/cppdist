/*
 * Copyright 2011 SZTAKI
 */
#include "sockhelp.h"
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>


#include "./socket.h"
#include "../../util/log.h"

//
// Socket
//

/*void Socket::SetNonBlocking() {
  int opts;

  opts = fcntl(socket_file_descriptor, F_GETFL);
  if (opts < 0) {
    perror("fcntl(F_GETFL)");
    exit(EXIT_FAILURE);
  }
  opts = (opts | O_NONBLOCK);
  if (fcntl(socket_file_descriptor, F_SETFL, opts) < 0) {
    perror("fcntl(F_SETFL)");
    exit(EXIT_FAILURE);
  }
  return;
}*/

Socket::Socket() {
  isClosed_ = false;
}

Socket::~Socket() {
  if (!isClosed_) {
    Close();
  }
}

bool Socket::IsClosed() {
  return isClosed_;
}

int Socket::GetFileDescriptor() {
  return socket_file_descriptor;
}

void Socket::Close() {
  if (!isClosed_) {
    close(socket_file_descriptor);
    isClosed_ = true;
  }
}

//
// ServerSocket
//

ServerSocket::ServerSocket() {
}

ServerSocket::~ServerSocket() {
}

ServerSocket *ServerSocket::Create(int port) {
  char service[10];
  sprintf(service, "%d", port);
  port = atoport(service, "tcp");

  ServerSocket *socket_obj = new ServerSocket;
  socket_obj->socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  socket_obj->port_ = port;
  if (socket_obj->socket_file_descriptor < 0) {
    throw ServerCreateError(port);
  }
  int reuse_addr = 1;
  /* So that we can re-bind to it without TIME_WAIT problems */
  setsockopt(socket_obj->socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
             sizeof(reuse_addr));

  struct sockaddr_in server_address; /* bind info structure */
  memset((char *) &server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = port;
  if (bind(socket_obj->socket_file_descriptor, (struct sockaddr *) &server_address,
           sizeof(server_address)) < 0) {
    close(socket_obj->socket_file_descriptor);
    throw BindError(port);
  }

  /* Set up queue for incoming connections. */
  listen(socket_obj->socket_file_descriptor, 5);

  return socket_obj;
}

int ServerSocket::GetPort() {
  return port_;
}

//
// SocketConnection
//

SocketConnection *ServerSocket::Accept() {
  SocketConnection *socket_res = new SocketConnection;
  int connection = accept(socket_file_descriptor, NULL, NULL);
  if (connection < 0) {
    throw AcceptError(port_);
  }
  socket_res->socket_file_descriptor = connection;
  if (socket_res == NULL) throw AcceptError(port_);
  return socket_res;
}

SocketConnection::SocketConnection() {
}

SocketConnection::~SocketConnection() {
}


SocketConnection *SocketConnection::Connect(string host, int port) {
  SocketConnection *res = new SocketConnection;
  res->port_ = port;
  res->host_ = host;
  char *netaddress = new char[host.size()+1];
  strcpy(netaddress, host.c_str());
  // TODO exception
  //fprintf(stderr, "port=%d\n", port);
  res->socket_file_descriptor = make_connection(port, netaddress);
  if (res->socket_file_descriptor == -1) {
    throw ConnectError(host, port);
  }
  //delete[]  netaddress;
  return res;
}

int SocketConnection::Send(int length, const char *buf) {
  return ::send(socket_file_descriptor, buf, length, MSG_NOSIGNAL);
}

int SocketConnection::SendCStr(const char *msg) {
  this->Send(strlen(msg) + 1, msg);
}

int SocketConnection::SendStr(const string &msg) {
  this->Send(msg.size() + 1, msg.c_str());
}

int SocketConnection::Recv(int limit, char* buf) {
  memset(buf, 0, limit);

  // status: the number of recieved bytes
  int status = ::recv(socket_file_descriptor, buf, limit, 0);

  if (status == -1) {
    return -1;
  } else if (status == 0) {
    return 0;
  } else {
    return status;
  }
}

int SocketConnection::GetPort() {
  return port_;
}

string SocketConnection::GetHost() {
  return host_;
}

//
// Selector
//

void Selector::Init(vector<SocketConnection *> sockets) {
  sockets_ = sockets;
  BuilFdSet();
}

void Selector::BuilFdSet() {
  /* First put together fd_set for select(), which will
     consist of the socket_file_descriptor veriable in case a new connection
     is coming in, plus all the sockets we have already
     accepted. */
  FD_ZERO(&socks_);
  max_fd_ = -1;
  for (unsigned int listnum = 0; listnum < sockets_.size(); listnum++) {
    Socket *socket = sockets_[listnum];

    // self socket is uninited
    if (socket == NULL) continue;

    if (!socket->IsClosed()) {
      int fd = socket->GetFileDescriptor();
      FD_SET(fd, &socks_);
      if (fd > max_fd_)
        max_fd_ = fd;
    }
  }
}

int Selector::RandStart () {
  timeval tim;
  gettimeofday(&tim, NULL);
  srand(tim.tv_usec);
  double x = (double)rand() * sockets_.size() / RAND_MAX;
  return (int) x;
}

SocketConnection *Selector::Select() {
  BuilFdSet();
  int readsocks = select(max_fd_ + 1, &socks_, (fd_set *) 0,
                         (fd_set *) 0, NULL);
  if (readsocks < 0) {
    throw SelectError();
    //return NULL;
  }
  if (readsocks == 0) {
    throw SelectError();
  } else {

    int rand_start = RandStart();
    for (int listnum = rand_start; listnum < (int)sockets_.size() + rand_start; listnum++) {
      SocketConnection *socket = sockets_[listnum%((int)sockets_.size())];

      // self socket is uninited
      if (socket == NULL) continue;
      if (FD_ISSET(socket->GetFileDescriptor(), &socks_)) {
        return socket;
      }
    }
  }
  // ide nem is johet
  return NULL;
}

int Selector::SelectIndex() {
  BuilFdSet();
  int readsocks = select(max_fd_ + 1, &socks_, (fd_set *) 0,
                         (fd_set *) 0, NULL);
  if (readsocks < 0) {
    throw SelectError();
  }
  if (readsocks == 0) {
    throw SelectError();
  } else {

    int rand_start = RandStart();
    for (int listnum = rand_start; listnum < (int)sockets_.size() + rand_start; listnum++) {
      SocketConnection *socket = sockets_[listnum%((int)sockets_.size())];

      // self socket is uninited
      if (socket == NULL) continue;
      if (FD_ISSET(socket->GetFileDescriptor(), &socks_)) {
        return listnum % (int)sockets_.size();
      }
    }
  }
  // ide nem is johet
  return -1;
}
