/*
 * test_cluster_setup.cpp
 *
 *  Created on: 2013.08.14.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <vector>

#include "../../main/algos/algo.h"
#include "../../main/algos/master.h"
#include "../../main/common/thread/node_thread.h"
#include "../common/mock_store_from_binary.h"
#include "mock_node.h"
#include "mock_inner_master.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

using std::vector;

namespace {

class ClusterSetupTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	ClusterSetupTest() {
  }

  virtual ~ClusterSetupTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST(ClusterSetupTest, test) {
	log4cpp::Appender *appender = new log4cpp::OstreamAppender("console", &std::cout);
	appender->setLayout(new log4cpp::BasicLayout());

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.setPriority(log4cpp::Priority::INFO);
	root.addAppender(appender);

	char localhost[1024] = "localhost";
	char test_cluster1[1024] = "/tmp/test_cluster1.log";
	char test_cluster2[1024] = "/tmp/test_cluster2.log";
	char test_input1[1024] = "/tmp/test_input1";
	char test_input2[1024] = "/tmp/test_input2";
	char master_log[1024] = "/tmp/test_master_log";

	Algo* algo1 = new Algo(localhost, 8000,
	  		8001, 6000, 1000, 2, 0, 0, 10);
	Algo* algo2 = new Algo(localhost, 8000,
	  		8002, 6000, 1000, 2, 1, 10, 10);

	vector<Slave> slaves;
  Slave s;
  strcpy(s.ip, localhost);
 // strcpy(s.path, test_input1);
  s.minNode = 0;
  s.numNode = 10;
  s.port = 8001;

  slaves.push_back(s);
  s.port = 8002;
  s.minNode = 10;
  s.numNode = 20;
  //strcpy(s.path, test_input2);
  slaves.push_back(s);

	Master* master = new Master(8000, &slaves, 20);
	MockInnerMaster* mockInnerMaster = new MockInnerMaster;
	string info1("/tmp/test_cluster1 0 0 10");
	string info2("/tmp/test_cluster2 10 0 10");
	stringstream ss1;
	stringstream ss2;
	ss1 << info1;
	ss2 << info2;
	//EXPECT_CALL(*mockInnerMaster, addInfoForNodes(ss2)).Times(1);


	master->setInnerMaster(mockInnerMaster);


  // mock objects
	MockNode* node1 = new MockNode;
	MockNode* node2 = new MockNode;
	algo1->setNode(node1);
	algo2->setNode(node2);

	//EXPECT_CALL(*node1, setPartitionIndex(0)).Times(1);
	//EXPECT_CALL(*node2, setPartitionIndex(1)).Times(1);
	//EXPECT_CALL(*node1, initData("/tmp/test_input1", 0, 10)).Times(1);
	//EXPECT_CALL(*node2, initData("/tmp/test_input2", 10, 20)).Times(1);
	EXPECT_CALL(*node1, initFromMaster("")).Times(1);
	EXPECT_CALL(*node2, initFromMaster("")).Times(1);

  SenderBuffer* senderBuffer1 = new SenderBuffer;
  SenderBuffer* senderBuffer2 = new SenderBuffer;
  algo1->setSenderBuffer(senderBuffer1);
  algo2->setSenderBuffer(senderBuffer2);

  SocketManager* socketManager1 = new SocketManager;
  SocketManager* socketManager2 = new SocketManager;
  algo1->setSocketManager(socketManager1);
  algo2->setSocketManager(socketManager2);

  MockStoreFromBinary* storeFromBinary1 = new MockStoreFromBinary;
  MockStoreFromBinary* storeFromBinary2 = new MockStoreFromBinary;
  algo1->setStoreFromBinary(storeFromBinary1);
  algo2->setStoreFromBinary(storeFromBinary2);

	char setup[1024] = "setup";
  NodeThread* master_thread = new NodeThread(master, setup);
  NodeThread* algo_thread1 = new NodeThread(algo1, setup);
  NodeThread* algo_thread2 = new NodeThread(algo2, setup);

  root.info("Start.");
  master_thread->start();
  algo_thread1->start();
  algo_thread2->start();

  master_thread->waitForThread();
  algo_thread1->waitForThread();
  algo_thread2->waitForThread();

  root.info("Finished threads.");
  delete node1;
  delete node2;
  delete mockInnerMaster;
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
