#include "../../../../main/common/util/util.h"
#include "../../../../main/common/components/socket/multicast_socket_manager.h"
#include "../../../../main/common/util/logger_factory.h"
#include <stdio.h>
#include <sstream>
#include <string>
#include "log4cpp/Category.hh"

using std::stringstream;
using std::string;

int main(int argc, char* argv[]) {
  Util util;
  LoggerFactory::initLogger("INFO", "CONSOLE", "");
  log4cpp::Category* logger =
      &log4cpp::Category::getInstance(std::string("MulticastSocketManagerListener"));

  int nodeIndex, startingHash, initMultiCastPort, clusterSize;
  char initMulticastHost[1024];
  char outIndices[1024];

  sscanf(argv[1], "%d", &nodeIndex);
  sscanf(argv[2], "%d", &startingHash);
  strcpy(initMulticastHost, argv[3]);
  sscanf(argv[4], "%d", &initMultiCastPort);
  sscanf(argv[5], "%d", &clusterSize);

  MulticastSocketManager manager(nodeIndex, startingHash,
      initMulticastHost, initMultiCastPort, clusterSize);
  manager.initConnectionsAlone();

  Selector* selector = manager.getSelector(0);
  int selectIndex = selector->SelectIndex();

  if (selectIndex < 0) {
    std::cout << "\n";
    return 1;
  }

  char msg[1024];
  int size = manager.recvFromNode(1024, msg, selectIndex);
  msg[size] = '\0';
  std::cout << msg << '\n';



  return 0;
}
