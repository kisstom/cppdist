#include "UDPMulticastPublisher.h"

int UDPMulticastPublisher::create(char* group, int port) {
  if ((socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    throw CreateError();
  }

  /* set up destination address */
  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=inet_addr(group);
  addr.sin_port=htons(port);

  return socket_file_descriptor;
}

int UDPMulticastPublisher::send(int limit, char* buffer) {
  int nbytes;
  nbytes = sendto(socket_file_descriptor, buffer, limit, 0,
      (struct sockaddr *) &addr, sizeof(addr));

  if (nbytes < 0) {
    throw SendError();
  }
  return nbytes;
}
