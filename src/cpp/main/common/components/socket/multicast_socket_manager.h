#ifndef MULTICAST_SOCKET_MANAGER_H_
#define MULTICAST_SOCKET_MANAGER_H_

#include "../../../../main/common/components/socket/UDPMulticastPublisher.h"
#include "../../../../main/common/components/socket/UDPMulticastReceiver.h"
#include <vector>
#include <gtest/gtest_prod.h>
#include "ip_index_maker.h"

using std::vector;

class MulticastSocketManager {
public:
  MulticastSocketManager(int, int, char*, int, int);
  void initSockets();
  void initPublishers();
  void initListeners();
private:
  vector<UDPMulticastPublisher*> publishers;
  vector<UDPMulticastReceiver*> listeners;
  IPIndexMaker ipIndexMaker;

  int nodeIndex;
  int startingHash;
  char initMulticastHost[1024];
  int initMultiCastPort;
  int clusterSize;

  // FRIEND TESTS
  FRIEND_TEST(TestMulticastSocketManager, createMulticastPublisher);
  FRIEND_TEST(TestMulticastSocketManager, createMulticastListener);
  FRIEND_TEST(TestMulticastSocketManager, createIPIndex);
};


#endif  // MULTICAST_SOCKET_MANAGER_H_
