#include "three_threaded_master.h"

void ThreeThreadedMaster::MakeNodeConnections() {
  sendMessageForAllNodes("HAVA NAGILA");
  WaitForNodes();
  sendMessageForAllNodes("HAVA NAGILA");
  WaitForNodes();
  sendMessageForAllNodes("HAVA NAGILA");
  WaitForNodes();
  sendMessageForAllNodes("HAVA NAGILA");
  WaitForNodes();
}
