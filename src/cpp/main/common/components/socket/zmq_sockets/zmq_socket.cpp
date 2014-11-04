#include "zmq_socket.h"


ZMQSocket::ZMQSocket(zmq::context_t* _context) {
  isClosed = true;
  context = _context;
  logger = &log4cpp::Category::getInstance(std::string("ZMQSocket"));
  socket = NULL;
}

bool ZMQSocket::IsClosed() {
  return isClosed;
}

void ZMQSocket::Close() {
  if (isClosed) return;

  int sc = zmq_close(socket);
  if (sc < 0) {
    logger->error("Error closing socket.");
    return;
  }

  isClosed = true;
}

void ZMQSocket::bind(char* host) {
  socket->bind(host);
}

void ZMQSocket::connect(char* host) {
  socket->connect(host);
}


int ZMQSocket::GetPort() {
  return -1;
}

int ZMQSocket::Send(int length, const char *buf) {
  return socket->send(buf, length);
}
int ZMQSocket::Recv(int limit, char* buf) {
  return socket->recv(buf, limit);
}

ZMQSocket::~ZMQSocket() {
  Close();
}

