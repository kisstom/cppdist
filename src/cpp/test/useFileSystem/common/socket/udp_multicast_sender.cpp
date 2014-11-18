#include "../../../../main/common/components/socket/UDPMulticastPublisher.h"
#include "../../../../main/common/util/util.h"
#include <cstdlib>

int main(int argc, char* argv[]) {
  char group[1024];
  strcpy(group, argv[1]);

  int port = atoi(argv[2]);
  int numMessages = atoi(argv[3]);
  int triggerPort = atoi(argv[4]);
  int messageSize = atoi(argv[5]);
  Util util;

  util.zmqSocketBlock(triggerPort);
  UDPMulticastPublisher publisher;
  publisher.create(group, port);

  for (int i = 0; i < numMessages; ++i) {
    publisher.send(messageSize, util.createBufferAtSize(messageSize, i));
  }

  return 0;
}
