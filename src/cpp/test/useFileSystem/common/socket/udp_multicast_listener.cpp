#include "../../../../main/common/components/socket/UDPMulticastReceiver.h"
#include "../../../../main/common/util/util.h"
#include <cstdlib>
#include <cstdio>

int main(int argc, char* argv[]) {
  char group[1024];
  strcpy(group, argv[1]);

  int port = atoi(argv[2]);
  int numMessages = atoi(argv[3]);
  int triggerPort = atoi(argv[4]);
  int messageSize = atoi(argv[5]);
  Util util;

  util.zmqSocketBlock(triggerPort);
  UDPMulticastReceiver receiver;
  receiver.connectToMulticastIp(group, port);

  char* buf = new char[messageSize + 1];
  int bytes;
  for (int i = 0; i < numMessages; ++i) {
    bytes = receiver.Recv(messageSize, buf);
    buf[bytes] = '\0';
    printf("%s\n", buf);
  }

  return 0;
}
