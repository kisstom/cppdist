#include <string.h>
#include "ConnectionError.h"
#include <assert.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

void ConnectionError::getIp(char* ip) {
    static struct ifreq ifreqs[32];
    struct ifconf ifconf;
    memset(&ifconf, 0, sizeof(ifconf));
    ifconf.ifc_req = ifreqs;
    ifconf.ifc_len = sizeof(ifreqs);

    int sd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sd >= 0);

    int r = ioctl(sd, SIOCGIFCONF, (char *)&ifconf);
    assert(r == 0);

    bool found = false;
    for(unsigned i = 0; i < ifconf.ifc_len/sizeof(struct ifreq); ++i)
    {
        if (strstr(ifreqs[i].ifr_name, "eth") != NULL) {
          strcpy(ip, inet_ntoa(((struct sockaddr_in *)&ifreqs[i].ifr_addr)->sin_addr));
          found = true;
          break;
        }
    }
    close(sd);
}

const char* ConnectionError::what() {
  return error_msg_;
}

SocketError::SocketError(int port): ConnectionError(), port_(port) {}

SelectError::SelectError() : ConnectionError() {
  char host[1024];
  getIp(host);
  sprintf(error_msg_, "Selecting error on host %s.", host);
}

ServerCreateError::ServerCreateError(int port) : SocketError(port) {
  char host[1024];
  getIp(host);
  sprintf(error_msg_, "Error creating server on host %s at port %d.", host, port_);
}

AcceptError::AcceptError(int port) : SocketError(port) {
  char host[1024];
  getIp(host);
  sprintf(error_msg_, "Error while wiating connection on host %s at port %d.", host, port_);
}

BindError::BindError(int port) : SocketError(port) {
  char host[1024];
  getIp(host);
  sprintf(error_msg_, "Binding error on host %s at port %d.", host, port_);
}

RecvError::RecvError(int port) : SocketError(port) {
  char host[1024];
  getIp(host);
  sprintf(error_msg_, "Receiving bad message on host %s at port %d.", host, port_);
}

CreateError::CreateError() : SocketError(0) {
  char host[1024];
  getIp(host);
  sprintf(error_msg_, "Receiving bad message on host %s at port %d.", host, port_);
}

SendError::SendError() : SocketError(0) {
  char host[1024];
  getIp(host);
  sprintf(error_msg_, "Receiving bad message on host %s at port %d.", host, port_);
}

ConnectError::ConnectError(string host, int port) : SocketError(port), host_(host) {
  char self_host[1024];
  getIp(self_host);
  sprintf(error_msg_, "Connect from host %s to host %s at port %d.",
      self_host, host_.c_str(), port_);
}

NotImplementedException::NotImplementedException(): msg("NotImplementedException") {}

const char* NotImplementedException::what() {
  return msg;
}

