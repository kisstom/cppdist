#include <gtest/gtest.h>
#include "../../main/common/components/socket/multicast_socket_manager.h"
#include <cmath>

class TestMulticastSocketManager: public ::testing::Test {
public:
  TestMulticastSocketManager() {}
};

TEST_F(TestMulticastSocketManager, constructorTest) {
  int nodeIndex = 0;
  int startingHash = 1000;
  char initMulticastHost[1024] = "225.0.0.";
  int initMultiCastPort = 7000;
  int clusterSize = 5;

  MulticastSocketManager manager(nodeIndex, startingHash,
      initMulticastHost, initMultiCastPort, clusterSize);
}

TEST_F(TestMulticastSocketManager, testIsLastBitOne) {
  int nodeIndex = 2;
  int startingHash = 1000;
  char initMulticastHost[1024] = "225.0.0.";
  int initMultiCastPort = 7000;
  int clusterSize = 5;

  MulticastSocketManager manager(nodeIndex, startingHash,
      initMulticastHost, initMultiCastPort, clusterSize);

  ASSERT_EQ(1, manager.getLastBit(5));
  ASSERT_EQ(0, manager.getLastBit(4));

  ASSERT_EQ(3, manager.getNumberOfZeros(5));
  ASSERT_EQ(4, manager.getNumberOfZeros(4));
  ASSERT_EQ(5, manager.getNumberOfZeros(0));
  ASSERT_EQ(0, manager.getNumberOfZeros(31));
}

TEST_F(TestMulticastSocketManager, createMulticastPublisher) {
  int nodeIndex = 2;
  int startingHash = 1000;
  char initMulticastHost[1024] = "225.0.0.";
  int initMultiCastPort = 7000;
  int clusterSize = 5;

  MulticastSocketManager manager(nodeIndex, startingHash,
      initMulticastHost, initMultiCastPort, clusterSize);

  int expectedPublisherSize = pow(2, clusterSize - 1) - 1;
  manager.initSockets();
  manager.initPublishers();
  manager.initListeners();
  ASSERT_EQ(expectedPublisherSize, (int) manager.publishers.size());

  for (int i = 0; i < (int) manager.publishers.size(); ++i) {
    ASSERT_TRUE(manager.publishers[i]->isConnected());
  }
}

TEST_F(TestMulticastSocketManager, createMulticastListener) {
  int nodeIndex = 2;
  int startingHash = 1000;
  char initMulticastHost[1024] = "225.0.0.";
  int initMultiCastPort = 7000;
  int clusterSize = 5;

  MulticastSocketManager manager(nodeIndex, startingHash,
      initMulticastHost, initMultiCastPort, clusterSize);

  int expectedListenerSize = pow(2, clusterSize - 1) - 1;
  manager.initSockets();
  manager.initListeners();

  ASSERT_EQ(expectedListenerSize, (int) manager.listeners.size());

  for (int i = 0; i < (int) manager.listeners.size(); ++i) {
    ASSERT_TRUE(manager.listeners[i]->isConnected());
  }
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
