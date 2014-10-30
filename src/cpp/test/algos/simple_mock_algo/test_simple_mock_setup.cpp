/*
 * test_simple_mock2.cpp
 *
 *  Created on: 2014.06.16.
 *      Author: kisstom
 */


#include <gtest/gtest.h>
#include "../../../main/algos/algo_components/test_simrank_odd_even_node_factory.h"
#include "../../../main/algos/algo_components/cluster.h"
#include "../algo_test_base.h"

class SimpleMockTestSetup: public AlgoTestBase {
protected:


  SimpleMockTestSetup() {
  }

  virtual ~SimpleMockTestSetup() {
  }

  virtual void SetUp() {
    initParams("SIMPLE_MOCK");
    initLogger();

    addFactory(2);
    addFactory(2);

    setUpBuilder();
    finalSetup();
  }

  void addFactory(int numSlaves) {
    TestSimrankOddEvenNodeFactory* nodeFactory = new TestSimrankOddEvenNodeFactory();
    vector<string> partString;
    AlgoTestBase::addNodeFactory(nodeFactory, partString, numSlaves);
  }

};

TEST_F(SimpleMockTestSetup, testSetup) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
  cluster.init();
  cluster.setUp();

  Algo* algo = cluster.getAlgo(0);
  ASSERT_TRUE(NULL != (algo->socketManager_->sender_sockets_)[1]);
  ASSERT_TRUE(NULL == (algo->socketManager_->sender_sockets_)[0]);
  ASSERT_TRUE(NULL != (algo->socketManager_->receiver_sockets_)[1]);
  ASSERT_TRUE(NULL == (algo->socketManager_->receiver_sockets_)[0]);
  ASSERT_EQ(slaveIndex_, (int) algo->senderBuffer_->pack_.size());
  ASSERT_EQ(slaveIndex_, (int) algo->senderBuffer_->pack_size_.size());
  ASSERT_EQ(sendLimit, algo->senderBuffer_->send_limit_);
  for (int i = 0; i < (int) algo->senderBuffer_->pack_size_.size(); ++i) {
    ASSERT_EQ(0, algo->senderBuffer_->pack_size_[i]);
  }
  ASSERT_EQ(slaveIndex_, (int) algo->storeFromBinary_->receiver_remains_.size());
  ASSERT_EQ(slaveIndex_, (int) algo->storeFromBinary_->remains_size_.size());
  for (int i = 0; i < (int) algo->storeFromBinary_->receiver_remains_.size(); ++i) {
    ASSERT_EQ(0, algo->storeFromBinary_->remains_size_[i]);
  }


  algo = cluster.getAlgo(1);
  ASSERT_TRUE(NULL != (algo->socketManager_->sender_sockets_)[0]);
  ASSERT_TRUE(NULL == (algo->socketManager_->sender_sockets_)[1]);
  ASSERT_TRUE(NULL != (algo->socketManager_->receiver_sockets_)[0]);
  ASSERT_TRUE(NULL == (algo->socketManager_->receiver_sockets_)[1]);
  ASSERT_EQ(slaveIndex_, (int) algo->senderBuffer_->pack_.size());
  ASSERT_EQ(slaveIndex_, (int) algo->senderBuffer_->pack_size_.size());
  ASSERT_EQ(sendLimit , algo->senderBuffer_->send_limit_);
  for (int i = 0; i < (int) algo->senderBuffer_->pack_size_.size(); ++i) {
    ASSERT_EQ(0, algo->senderBuffer_->pack_size_[i]);
  }
  ASSERT_EQ(slaveIndex_, (int) algo->storeFromBinary_->receiver_remains_.size());
  ASSERT_EQ(slaveIndex_, (int) algo->storeFromBinary_->remains_size_.size());
  for (int i = 0; i < (int) algo->storeFromBinary_->receiver_remains_.size(); ++i) {
    ASSERT_EQ(0, algo->storeFromBinary_->remains_size_[i]);
  }
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
