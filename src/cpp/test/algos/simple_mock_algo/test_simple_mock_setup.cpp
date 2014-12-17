#include "gtest/gtest.h"
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
    int numPart = 3;
    //params_["MULTI"] = "1";
    initParams("SIMPLE_MOCK");
    initLogger();

    addFactory(numPart);
    addFactory(numPart);
    addFactory(numPart);

    setUpBuilder();
    finalSetup();
  }

  void addFactory(int numSlaves) {
    TestSimrankOddEvenNodeFactory* nodeFactory = new TestSimrankOddEvenNodeFactory();
    vector<string> partString;
    AlgoTestBase::addNodeFactory(nodeFactory, partString, numSlaves);
  }

  void checkAlgo(Algo* algo) {
    /*ASSERT_EQ(slaveIndex_, (int) algo->socketManager_->sender_sockets_.size());
    ASSERT_EQ(slaveIndex_, (int) algo->socketManager_->receiver_sockets_.size());

    int index = algo->slave_index_;
    for (int i = 0; i < (int) algo->socketManager_->sender_sockets_.size(); ++i) {
      if (i == index) {
        ASSERT_TRUE(NULL == (algo->socketManager_->receiver_sockets_)[i]);
        ASSERT_TRUE(NULL == (algo->socketManager_->sender_sockets_)[i]);
      } else {
        ASSERT_TRUE(NULL != (algo->socketManager_->sender_sockets_)[i]);
        ASSERT_TRUE(NULL != (algo->socketManager_->receiver_sockets_)[i]);
      }
    }*/

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
  }

};

TEST_F(SimpleMockTestSetup, testSetup) {
  Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_, &clusterNodeParams);
  cluster.init();
  cluster.setUp();

  Algo* algo = cluster.getAlgo(0);
  checkAlgo(algo);

  algo = cluster.getAlgo(1);
  checkAlgo(algo);
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
