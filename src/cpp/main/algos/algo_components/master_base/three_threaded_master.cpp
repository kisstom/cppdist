#include "three_threaded_master.h"

ThreeThreadedMaster::ThreeThreadedMaster(int master_port, vector<Slave>* slaves, long numNodes):
MasterBase(master_port, slaves, numNodes) {}

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
