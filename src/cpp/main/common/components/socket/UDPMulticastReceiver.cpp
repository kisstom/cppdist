#include "UDPMulticastReceiver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void UDPMulticastReceiver::connectToMulticastIp(char* group, int port) {
  u_int yes=1;
  struct ip_mreq mreq;

  /* create what looks like an ordinary UDP socket */
  if ((socket_file_descriptor=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
    //perror("socket");
    return;
  }

  /* allow multiple sockets to use the same PORT number */
  if (setsockopt(socket_file_descriptor,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
    //perror("Reusing ADDR failed");
    return;
  }

  /* set up destination address */
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  addrlen = sizeof(&addr);

  /* bind to receive address */
  if (bind(socket_file_descriptor, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    //perror("bind");
    return;
  }

  /* use setsockopt() to request that the kernel join a multicast group */
  mreq.imr_multiaddr.s_addr = inet_addr(group);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(socket_file_descriptor,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
    //perror("setsockopt");
    return;
  }

  return;
}

int UDPMulticastReceiver::Recv(int limit, char* buffer) {
  int nbytes;

  if ((nbytes = recvfrom(socket_file_descriptor, buffer, limit, 0,
      (struct sockaddr *) &addr, (socklen_t*) &addrlen)) < 0) {
    //perror("recvfrom");
    return -1;
  }

  return nbytes;
}
