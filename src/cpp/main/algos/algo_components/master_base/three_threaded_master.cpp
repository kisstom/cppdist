#include "three_threaded_master.h"

ThreeThreadedMaster::ThreeThreadedMaster(int master_port, vector<Slave>* slaves):
MasterBase(master_port, slaves) {}

void ThreeThreadedMaster::MakeNodeConnections() {
  char fakeMessage[1024] = "HAVA NAGILA";
  // Waiting for initing publishers.
  WaitForNodes();

  // Inits listeners.
  sendMessageForAllNodes(fakeMessage);

  // ... initing listeners...
  // ... initing publishers ...
  // waiting for publishers
  WaitForNodes();

  // initing subscribers
  sendMessageForAllNodes(fakeMessage);

  // waiting for subscribers
  WaitForNodes();
}
