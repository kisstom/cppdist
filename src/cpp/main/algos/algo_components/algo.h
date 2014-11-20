/*
 * algo.h
 *
 *  Created on: 2013.08.09.
 *      Author: kisstom
 */

#ifndef ALGO_H_
#define ALGO_H_

#include <vector>

#include "node.h"
#include "runnable.h"
#include "../../common/components/sender_buffer.h"
#include "../../common/components/socket/socket_manager.h"
#include "../../common/components/socket/master_socket_manager.h"
#include "../../common/components/deserializer.h"
#include "../../common/components/store_from_binary.h"
#include "../../common/thread/receiver_thread.h"
#include "../../common/thread/sender_thread.h"
#include "../../common/thread/thread_manager.h"

#include <sstream>
#include "log4cpp/PatternLayout.hh"
#include <limits.h>
#include <gtest/gtest_prod.h>

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

//log4cpp::Category& root = log4cpp::Category::getRoot();
using std::vector;

class Algo : public Runnable {
public:
	Algo(char* master_host, int master_port, int slave_port,
			int send_limit, long all_node, int num_slaves, int slave_index,
			long num_nodes, long min_node);
	void run();
	//void startLogger();
	void setIp();
	void initClient();
	void connectToMaster();
	void initFromMaster();
	void initContainers();
	void initConnections();
	void runThreads();
	int getPartitionIndex(long);
	void receiver();
	bool storeFromBinary(int);
  bool setUp();
  void sendAndSignal(int);
  int getSlaveIndex();
  long getPartitionStartNode(int part_index);
  short getNumberOfPartitions();
  void final();
  long getAllNodes();
  long getMinnode();
  long getNumberOfPartitionNodes();

	void setNode(Node *);
  void setSocketManager(SocketManager*);
  void setMasterSocketManager(MasterSocketManager* manager);
  void setStoreFromBinary(StoreFromBinary*);
  void setSenderBuffer(SenderBuffer*);
  virtual ~Algo();
private:
	char ip_[1024];
	char master_host_[1024];
	int master_port_;
	int slave_port_;
  long all_node_;
  long num_nodes_;
  int current_iteration_;

	vector<long> partition_min_node_;
	int num_slaves_;
	int send_limit_;
	int part_index_;
  long min_node_;
  int slave_index_;

	SocketManager* socketManager_;
	MasterSocketManager* masterSocketManager_;

	SenderBuffer* senderBuffer_;
	Node* node_;
	StoreFromBinary* storeFromBinary_;
	log4cpp::Category* logger_;

	FRIEND_TEST(SimpleMockTestSetup, testSetup);
	friend class SimpleMockTestSetup;
};



#endif /* ALGO_H_ */
