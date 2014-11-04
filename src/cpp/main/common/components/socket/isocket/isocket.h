#ifndef ISOCKET_H_
#define ISOCKET_H_

#include <string>
using std::string;

class ISocket {
public:
  virtual bool IsClosed() = 0;
  virtual void Close() = 0;
  virtual int GetPort() = 0;
  virtual int Send(int length, const char *buf) = 0;
  virtual int Recv(int limit, char* buf) = 0;
  virtual ~ISocket() {}
};


#endif  // ISOCKET_H_
