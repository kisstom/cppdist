#ifndef MULTICAST_SOCKET_MANAGER_H_
#define MULTICAST_SOCKET_MANAGER_H_

#include "../../../../main/common/components/socket/UDPMulticastPublisher.h"
#include "../../../../main/common/components/socket/UDPMulticastReceiver.h"
#include <vector>
#include <gtest/gtest_prod.h>
#include "ip_index_maker.h"
#include "isocket/isocket_manager.h"
#include "master_socket_manager.h"
#include "log4cpp/Category.hh"

using std::vector;

class MulticastSocketManager : public ISocketManager  {
public:
  MulticastSocketManager(int, int, char*, int, int);
  int recvFromNode(int, char*, int);
  void sendToNode(int, char*, int);
  void initConnections();
  void initSockets(int);
  void initClient(int) {}
  void setMasterSocketManager(MasterSocketManager*);
  Selector* getSelector();

  ~MulticastSocketManager();
private:
  void initPublishers();
  void initListeners();

  vector<UDPMulticastPublisher*> publishers;
  vector<SocketConnection*> listeners;
  MasterSocketManager* masterSocketManager;

  IPIndexMaker ipIndexMaker;

  int nodeIndex;
  int startingHash;
  char initMulticastHost[1024];
  int initMultiCastPort;
  int clusterSize;
  log4cpp::Category* logger;

  // FRIEND TESTS
  FRIEND_TEST(TestMulticastSocketManager, createMulticastPublisher);
  FRIEND_TEST(TestMulticastSocketManager, createMulticastListener);
  FRIEND_TEST(TestMulticastSocketManager, createIPIndex);
};


#endif  // MULTICAST_SOCKET_MANAGER_H_
