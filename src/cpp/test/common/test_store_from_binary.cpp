/*
 * test_store_from_binary.cpp
 *
 *  Created on: 2013.08.08.
 *      Author: kisstom
 */

#include "gtest/gtest.h"
#include "../../main/common/components/store_from_binary.h"
#include "../../main/algos/simrank_update/simrank_update_deserializer.h"
#include "../../main/common/components/serializer.h"
#include "../algos/simrank_update/mock_simrank_update_node.h"
#include <string.h>

namespace {

class TestStoreFromBinary: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	TestStoreFromBinary() {
  }

  virtual ~TestStoreFromBinary() {
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

TEST(TestStoreFromBinary, test) {
	StoreFromBinary storeFromBinary;
	SimrankUpdateDeserializer deserializer;
	MockSimrankUpdateNode* mockNode = new  MockSimrankUpdateNode();
	deserializer.node_ = mockNode;

	storeFromBinary.setDeserializer(&deserializer);
	storeFromBinary.resizeSocketNum(3);
	storeFromBinary.setBufferCapacity(1024);

	RandomWalk rw;
	rw.edge_num = 2;
	rw.fp_index = 5;
	rw.buf[0] = 1234;
	rw.buf[1] = 51;
	rw.buf[2] = 88888;

	char* buf = storeFromBinary.receiver_remains_[0];
	Serializer serializer;
	int s = 0;
	s += serializer.setBreak(buf + s);
	s += serializer.store(buf + s, rw.edge_num);
	s += serializer.store(buf + s, rw.buf[0]);
	s += serializer.store(buf + s, rw.buf[1]);
	s += serializer.store(buf + s, rw.buf[2]);
	s += serializer.store(buf + s, rw.fp_index);
	s += serializer.setFinish(buf + s);
	storeFromBinary.remains_size_[0] = 30;

	ASSERT_EQ(3, storeFromBinary.getSocketSize());
	//ASSERT_EQ(1024, storeFromBinary.getRemainCapacity());

	int index = 0;
	EXPECT_CALL(*mockNode, update(&deserializer.buffer_)).Times(1);
	bool cont = storeFromBinary.storeFromIndex(index);
	ASSERT_FALSE(cont);

	ASSERT_EQ(2, deserializer.buffer_.edge_num);
	ASSERT_EQ(5, deserializer.buffer_.fp_index);
	ASSERT_EQ(1234, deserializer.buffer_.buf[0]);
	ASSERT_EQ(51, deserializer.buffer_.buf[1]);
	ASSERT_EQ(88888, deserializer.buffer_.buf[2]);
	//ASSERT_EQ((unsigned) 0, storeFromBinary.remains_size_[0]);

	delete mockNode;
}

TEST(TestStoreFromBinary, test2) {
	StoreFromBinary storeFromBinary;
	SimrankUpdateDeserializer deserializer;
	MockSimrankUpdateNode* mockNode = new  MockSimrankUpdateNode();
	deserializer.node_ = mockNode;

	storeFromBinary.setDeserializer(&deserializer);
	storeFromBinary.resizeSocketNum(3);
	storeFromBinary.setBufferCapacity(1024);

	RandomWalk rw;
	rw.edge_num = 2;
	rw.fp_index = 5;
	rw.buf[0] = 1234;
	rw.buf[1] = 51;
	rw.buf[2] = 88888;

	RandomWalk rw2;
	rw2.edge_num = 3;
	rw2.fp_index = 11111;
	rw2.buf[0] = 7842;
	rw2.buf[1] = 765;
	rw2.buf[2] = 88111;
	rw2.buf[3] = 11999;

	char* buf = storeFromBinary.receiver_remains_[0];
	Serializer serializer;
	int s = 0;
	s += serializer.setBreak(buf + s);
	s += serializer.store(buf + s, rw.edge_num);
	s += serializer.store(buf + s, rw.buf[0]);
	s += serializer.store(buf + s, rw.buf[1]);
	s += serializer.store(buf + s, rw.buf[2]);
	s += serializer.store(buf + s, rw.fp_index);
  s += serializer.setBreak(buf + s);
	s += serializer.store(buf + s, rw2.edge_num);
	s += serializer.store(buf + s, rw2.buf[0]);
	s += serializer.store(buf + s, rw2.buf[1]);
	s += serializer.store(buf + s, rw2.buf[2]);
	s += serializer.store(buf + s, rw2.buf[3]);
	s += serializer.store(buf + s, rw2.fp_index);
	s += serializer.setFinish(buf + s);
	storeFromBinary.remains_size_[0] = 67;

	ASSERT_EQ(3, storeFromBinary.getSocketSize());
	//ASSERT_EQ(1024, storeFromBinary.getRemainCapacity());

	int index = 0;
	EXPECT_CALL(*mockNode, update(&deserializer.buffer_)).Times(2);
	bool cont = storeFromBinary.storeFromIndex(index);
	ASSERT_FALSE(cont);

	ASSERT_EQ(3, deserializer.buffer_.edge_num);
	ASSERT_EQ(11111, deserializer.buffer_.fp_index);
	ASSERT_EQ(7842, deserializer.buffer_.buf[0]);
	ASSERT_EQ(765, deserializer.buffer_.buf[1]);
	ASSERT_EQ(88111, deserializer.buffer_.buf[2]);
	ASSERT_EQ(11999, deserializer.buffer_.buf[3]);

	// Because of replacing client socket manager.
	//ASSERT_EQ((unsigned) 0, storeFromBinary.remains_size_[0]);

	delete mockNode;
}

TEST(TestStoreFromBinary, noInTheMiddle) {
	StoreFromBinary storeFromBinary;
	SimrankUpdateDeserializer deserializer;
	MockSimrankUpdateNode* mockNode = new  MockSimrankUpdateNode();
	deserializer.node_ = mockNode;

	storeFromBinary.setDeserializer(&deserializer);
	storeFromBinary.resizeSocketNum(3);
	storeFromBinary.setBufferCapacity(1024);

	RandomWalk rw;
	rw.edge_num = 2;
	rw.fp_index = 5;
	rw.buf[0] = 1234;
	rw.buf[1] = 51;
	rw.buf[2] = 88888;

	RandomWalk rw2;
	rw2.edge_num = 3;
	rw2.fp_index = 11111;
	rw2.buf[0] = 7842;
	rw2.buf[1] = 765;
	rw2.buf[2] = 88111;
	rw2.buf[3] = 11999;

	char* buf = storeFromBinary.receiver_remains_[0];
	Serializer serializer;
	int s = 0;
	s += serializer.setBreak(buf + s);
	s += serializer.store(buf + s, rw.edge_num);
	s += serializer.store(buf + s, rw.buf[0]);
	s += serializer.store(buf + s, rw.buf[1]);
	s += serializer.store(buf + s, rw.buf[2]);
	s += serializer.store(buf + s, rw.fp_index);
  s += serializer.setBreak(buf + s);
	s += serializer.store(buf + s, rw2.edge_num);
	s += serializer.store(buf + s, rw2.buf[0]);
	s += serializer.store(buf + s, rw2.buf[1]);
	s += serializer.store(buf + s, rw2.buf[2]);
	s += serializer.store(buf + s, rw2.buf[3]);
	s += serializer.store(buf + s, rw2.fp_index);
	s += serializer.setBreak(buf + s);
	s += serializer.store(buf + s, rw2.edge_num);
	s += serializer.store(buf + s, rw2.buf[0]);
	s += serializer.store(buf + s, rw2.buf[1]);
  // according to the previously added items
	storeFromBinary.remains_size_[0] = 85;

	ASSERT_EQ(3, storeFromBinary.getSocketSize());
	//ASSERT_EQ(1024, storeFromBinary.getRemainCapacity());

	int index = 0;
	EXPECT_CALL(*mockNode, update(&deserializer.buffer_)).Times(2);
	bool cont = storeFromBinary.storeFromIndex(index);
	ASSERT_TRUE(cont);

	ASSERT_EQ(3, deserializer.buffer_.edge_num);
	ASSERT_EQ(11111, deserializer.buffer_.fp_index);
	ASSERT_EQ(7842, deserializer.buffer_.buf[0]);
	ASSERT_EQ(765, deserializer.buffer_.buf[1]);
	ASSERT_EQ(88111, deserializer.buffer_.buf[2]);
	ASSERT_EQ(11999, deserializer.buffer_.buf[3]);
	ASSERT_EQ((unsigned) 19, storeFromBinary.remains_size_[0]);

	delete mockNode;
}
TEST(TestStoreFromBinary, noInTheMiddle2) {
	StoreFromBinary storeFromBinary;
	SimrankUpdateDeserializer deserializer;
	MockSimrankUpdateNode* mockNode = new  MockSimrankUpdateNode();
	deserializer.node_ = mockNode;

	storeFromBinary.setDeserializer(&deserializer);
	storeFromBinary.resizeSocketNum(3);
	storeFromBinary.setBufferCapacity(1024);

	RandomWalk rw;
	rw.edge_num = 2;
	rw.fp_index = 5;
	rw.buf[0] = 1234;
	rw.buf[1] = 51;
	rw.buf[2] = 88888;

	RandomWalk rw2;
	rw2.edge_num = 3;
	rw2.fp_index = 11111;
	rw2.buf[0] = 7842;
	rw2.buf[1] = 765;
	rw2.buf[2] = 88111;
	rw2.buf[3] = 11999;

	char* buf = storeFromBinary.receiver_remains_[0];
	Serializer serializer;
	int s = 0;
	s += serializer.setBreak(buf + s);
	s += serializer.store(buf + s, rw.edge_num);
	s += serializer.store(buf + s, rw.buf[0]);
	s += serializer.store(buf + s, rw.buf[1]);
	s += serializer.store(buf + s, rw.buf[2]);
	s += serializer.store(buf + s, rw.fp_index);
  s += serializer.setBreak(buf + s);
	s += serializer.store(buf + s, rw2.edge_num);
	s += serializer.store(buf + s, rw2.buf[0]);
	s += serializer.store(buf + s, rw2.buf[1]);
	s += serializer.store(buf + s, rw2.buf[2]);
	s += serializer.store(buf + s, rw2.buf[3]);
	s += serializer.store(buf + s, rw2.fp_index);
  // according to the previously added items
	storeFromBinary.remains_size_[0] = 66;

	ASSERT_EQ(3, storeFromBinary.getSocketSize());
	//ASSERT_EQ(1024, storeFromBinary.getRemainCapacity());

	int index = 0;
	EXPECT_CALL(*mockNode, update(&deserializer.buffer_)).Times(2);
	bool cont = storeFromBinary.storeFromIndex(index);
	ASSERT_TRUE(cont);

	ASSERT_EQ(3, deserializer.buffer_.edge_num);
	ASSERT_EQ(11111, deserializer.buffer_.fp_index);
	ASSERT_EQ(7842, deserializer.buffer_.buf[0]);
	ASSERT_EQ(765, deserializer.buffer_.buf[1]);
	ASSERT_EQ(88111, deserializer.buffer_.buf[2]);
	ASSERT_EQ(11999, deserializer.buffer_.buf[3]);
	ASSERT_EQ((unsigned) 0, storeFromBinary.remains_size_[0]);

	delete mockNode;
}


}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


