#include "../../../../main/common/util/util.h"
#include "../../../../main/common/util/logger_factory.h"
#include "../../../../main/common/components/socket/multicast_socket_manager.h"
#include "../../../../main/common/components/socket/multicast_helper.h"
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
      &log4cpp::Category::getInstance(std::string("MulticastSocketManagerPublisher"));


  int nodeIndex, startingHash, initMultiCastPort, clusterSize;
  char initMulticastHost[1024];
  char outIndices[1024];

  sscanf(argv[1], "%d", &nodeIndex);
  sscanf(argv[2], "%d", &startingHash);
  strcpy(initMulticastHost, argv[3]);
  sscanf(argv[4], "%d", &initMultiCastPort);
  sscanf(argv[5], "%d", &clusterSize);
  strcpy(outIndices, argv[6]);
  logger->info("%s\n", string(outIndices).c_str());

  MulticastSocketManager manager(nodeIndex, startingHash,
      initMulticastHost, initMultiCastPort, clusterSize);
  manager.initConnectionsAlone();

  MulticastHelper helper(nodeIndex);

  vector<string> indices = util.split(string(outIndices), ',');
  vector<short> shortIndices =  util.convertToShort(indices);

  short* shortArrIndices = new short[shortIndices.size() + 1];
  shortArrIndices[0] = (short) shortIndices.size();
  for (int i = 0; i < (int) shortIndices.size(); ++i) {
    shortArrIndices[i + 1] = shortIndices[i];
    logger->info("%hd ", shortIndices[i]);
  }

  bool foo;
  int hashIndex = helper.publishHashId(shortArrIndices, &foo);

  logger->info("Hash index %d", hashIndex);
  manager.sendToNode(5, "hello", hashIndex);


  return 0;
}
