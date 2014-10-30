/*
 * test_sender_buffer.cpp
 *
 *  Created on: 2013.08.01.
 *      Author: kisstom
 */
#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "../../main/common/components/sender_buffer.h"
#include "mock_socket_manager.h"

using std::vector;
using std::string;

//namespace {

class SenderBufferTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SenderBufferTest() {
  }

  virtual ~SenderBufferTest() {
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
//}

TEST(SenderBufferTest, test) {
  SenderBuffer senderBuffer;
  senderBuffer.resizeBufferNum(1);
  senderBuffer.resizeBuffers(1024);

  MockSocketManager* manager = new MockSocketManager;
  senderBuffer.setSocketManager(manager);
  EXPECT_CALL(*manager, sendToNode(14, senderBuffer.pack_[0], 0)).Times(1);

  ASSERT_EQ(0, senderBuffer.getBufferSize(0));
  int stored = 0;

  int bufferIndex = 0;
  long longToStore = 1231241;
  long readFromBuffer;
  senderBuffer.store(bufferIndex, longToStore);
  ASSERT_EQ(8, senderBuffer.getBufferSize(bufferIndex));
  memcpy(&readFromBuffer, senderBuffer.pack_[bufferIndex], sizeof(longToStore));
  ASSERT_EQ(longToStore, readFromBuffer);

  int intToStore = 1231241;
  int readIntFromBuffer;
  stored += sizeof(longToStore);
  senderBuffer.store(bufferIndex, intToStore);
  ASSERT_EQ(12, senderBuffer.getBufferSize(bufferIndex));
  memcpy(&readIntFromBuffer, senderBuffer.pack_[bufferIndex] + stored, sizeof(intToStore));
  ASSERT_EQ(intToStore, readIntFromBuffer);

  short shortToStore = 4096;
  short readShortFromBuffer;
  stored += sizeof(intToStore);
  senderBuffer.store(bufferIndex, shortToStore);
  ASSERT_EQ(14, senderBuffer.getBufferSize(bufferIndex));
  memcpy(&readShortFromBuffer, senderBuffer.pack_[bufferIndex] + stored, sizeof(shortToStore));
  ASSERT_EQ(shortToStore, readShortFromBuffer);

  senderBuffer.emptyBuffer(bufferIndex);
  ASSERT_EQ(0, senderBuffer.getBufferSize(0));

  delete manager;
}

TEST(SenderBufferTest, test2) {
	SenderBuffer senderBuffer;
	senderBuffer.resizeBufferNum(1);
	senderBuffer.resizeBuffers(32);
	ASSERT_TRUE(senderBuffer.canAdd(0, 31));
	ASSERT_TRUE(senderBuffer.canAdd(0, 32));
	ASSERT_FALSE(senderBuffer.canAdd(0, 33));
}

TEST(SenderBufferTest, testEmpty) {
	SenderBuffer senderBuffer;
	senderBuffer.resizeBufferNum(1);
	senderBuffer.resizeBuffers(32);

	MockSocketManager* manager = new MockSocketManager;
	senderBuffer.setSocketManager(manager);
	EXPECT_CALL(*manager, sendToNode(sizeof(long), senderBuffer.pack_[0], 0)).Times(1);

	long a = 1;
	senderBuffer.store(0, a);
	senderBuffer.emptyBuffer(0);

	delete manager;
}

TEST(SenderBufferTest, testSep) {
  SenderBuffer senderBuffer;
  senderBuffer.resizeBufferNum(1);
  senderBuffer.resizeBuffers(32);

  long a = 2234567890;
  senderBuffer.store(0, a);
  senderBuffer.setBreak(0);
  ASSERT_EQ(9, senderBuffer.pack_size_[0]);

  senderBuffer.setFinish(0);
  ASSERT_EQ(10, senderBuffer.pack_size_[0]);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


