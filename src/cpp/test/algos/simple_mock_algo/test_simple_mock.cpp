/*
 * test_simple_mock.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include "../../../main/algos/algo.h"
#include "../../../main/algos/master.h"
#include "../../../main/common/thread/node_thread.h"
#include "../../algos/simple_mock_algo/simple_mock_node.h"
#include "../../algos/simple_mock_algo/simple_mock_deserializer.h"
#include "../../algos/simple_mock_algo/simple_mock_inner_master.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

namespace {

class SimpleMockTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SimpleMockTest() {
  }

  virtual ~SimpleMockTest() {
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

TEST(SimpleMockTest, test) {
	log4cpp::Appender *appender = new log4cpp::OstreamAppender("console", &std::cout);
	appender->setLayout(new log4cpp::BasicLayout());

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.setPriority(log4cpp::Priority::INFO);
	root.addAppender(appender);

	char localhost[1024] = "localhost";
	char test_cluster1[1024] = "/tmp/test_cluster1";
	char test_cluster2[1024] = "/tmp/test_cluster2";
	char test_input1[1024] = "/tmp/test_input1";
	char test_input2[1024] = "/tmp/test_input2";
  char master_log[1024] = "/tmp/test_master_log";

	Algo* algo1 = new Algo(localhost, 8000,
			8001,  6000, 1000, 2, 0, 10, 0);
	Algo* algo2 = new Algo(localhost, 8000,
			8002,  6000, 1000, 2, 1, 10, 10);

	vector<Slave> slaves;
	Slave s;
	strcpy(s.ip, localhost);
	//strcpy(s.path, test_input1);
	s.minNode = 0;
	s.port = 8001;

	slaves.push_back(s);
	s.port = 8002;
	s.minNode = 10;
	//strcpy(s.path, test_input2);
	slaves.push_back(s);

	Master* master = new Master(8000, &slaves, 20);
	SimpleMockInnerMaster* mockInnerMaster = new SimpleMockInnerMaster;
	master->setInnerMaster(mockInnerMaster);
	mockInnerMaster->setMaster(master);

	SimpleMockNode* node1 = new SimpleMockNode;
	SimpleMockNode* node2 = new SimpleMockNode;
	algo1->setNode(node1);
	algo2->setNode(node2);
	node1->setAlgo(algo1);
	node2->setAlgo(algo2);

	SenderBuffer* senderBuffer1 = new SenderBuffer;
	SenderBuffer* senderBuffer2 = new SenderBuffer;
	algo1->setSenderBuffer(senderBuffer1);
	algo2->setSenderBuffer(senderBuffer2);

	SocketManager* socketManager1 = new SocketManager;
	SocketManager* socketManager2 = new SocketManager;
	algo1->setSocketManager(socketManager1);
	algo2->setSocketManager(socketManager2);

	StoreFromBinary* storeFromBinary1 = new StoreFromBinary;
	SimpleMockDeserializer* deserializer1 = new SimpleMockDeserializer;
	storeFromBinary1->setDeserializer(deserializer1);
	deserializer1->setNode(node1);
	algo1->setStoreFromBinary(storeFromBinary1);
	StoreFromBinary* storeFromBinary2 = new StoreFromBinary;
	SimpleMockDeserializer* deserializer2 = new SimpleMockDeserializer;
	storeFromBinary2->setDeserializer(deserializer2);
	deserializer2->setNode(node2);
	algo2->setStoreFromBinary(storeFromBinary2);

	char setup[1024] = "setup";
	NodeThread* master_thread = new NodeThread(master, setup);
	NodeThread* algo_thread1 = new NodeThread(algo1, setup);
	NodeThread* algo_thread2 = new NodeThread(algo2, setup);

	root.info("Starting threadded setup.");
	master_thread->start();
	algo_thread1->start();
	algo_thread2->start();

	master_thread->waitForThread();
	algo_thread1->waitForThread();
	algo_thread2->waitForThread();

	root.info("Finished threads.");
	delete master_thread;
	delete algo_thread1;
	delete algo_thread2;

	char run[1024] = "run";
	master_thread = new NodeThread(master, run);
	algo_thread1 = new NodeThread(algo1, run);
	algo_thread2 = new NodeThread(algo2, run);

	root.info("Starting threadded run.");
	master_thread->start();
	algo_thread1->start();
	algo_thread2->start();

	master_thread->waitForThread();
	algo_thread1->waitForThread();
	algo_thread2->waitForThread();

	delete senderBuffer1;
	delete senderBuffer2;
	delete socketManager1;
	delete socketManager2;
	delete storeFromBinary1;
	delete storeFromBinary2;
	delete deserializer1;
	delete deserializer2;
	delete node1;
	delete node2;
	delete algo1;
	delete algo2;
	delete master;
	delete mockInnerMaster;
  delete master_thread;
  delete algo_thread1;
  delete algo_thread2;

	root.info("Finished threads.");
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
