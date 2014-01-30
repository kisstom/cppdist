/*
 * test_simrank_updated_serializer.cpp
 *
 *  Created on: 2013.08.08.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include "../../../main/algos/simrank_update/simrank_update_deserializer.h"
#include "../../../main/common/components/serializer.h"
#include "../../algos/simrank_update/mock_simrank_update_node.h"

namespace {

class TestSimrankUpdateDeserializer: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	TestSimrankUpdateDeserializer() {
  }

  virtual ~TestSimrankUpdateDeserializer() {
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

TEST(TestSimrankUpdateDeserializer, testStore) {
	RandomWalk rw;
	rw.edge_num = 2;
	rw.fp_index = 5;
	rw.buf[0] = 1234;
	rw.buf[1] = 51;
	rw.buf[2] = 88888;

	char buf[1024];

	Serializer serializer;
	int s = 0;
	s += serializer.store(buf + s, rw.edge_num);
	s += serializer.store(buf + s, rw.buf[0]);
	s += serializer.store(buf + s, rw.buf[1]);
	s += serializer.store(buf + s, rw.buf[2]);
	s += serializer.store(buf + s, rw.fp_index);

	SimrankUpdateDeserializer deserializer;
	int stored = deserializer.storeFromBinary(buf, 1024);
	int expected_store = sizeof(long) * 3 + sizeof(short) * 2;
	ASSERT_EQ(2, deserializer.buffer_.edge_num);
	ASSERT_EQ(5, deserializer.buffer_.fp_index);
	ASSERT_EQ(1234, deserializer.buffer_.buf[0]);
	ASSERT_EQ(51, deserializer.buffer_.buf[1]);
	ASSERT_EQ(88888, deserializer.buffer_.buf[2]);

	ASSERT_EQ(expected_store, stored);
}

TEST(TestSimrankUpdateDeserializer, testUpdate) {
	SimrankUpdateDeserializer deserializer;
	deserializer.buffer_.edge_num = 2;
	deserializer.buffer_.fp_index = 5;
	deserializer.buffer_.buf[0] = 1234;
	deserializer.buffer_.buf[1] = 51;
	deserializer.buffer_.buf[2] = 88888;

	MockSimrankUpdateNode* mockNode = new  MockSimrankUpdateNode();
	EXPECT_CALL(*mockNode, update(&deserializer.buffer_)).Times(1);
	deserializer.node_ = mockNode;
	deserializer.update(0);

	delete mockNode;
}

TEST(TestSimrankUpdateDeserializer, testBufferSize) {
	SimrankUpdateDeserializer deserializer;
	int numEdges = 5;
	int expected_size = (numEdges + 1) * sizeof(long) + 2 * sizeof(short);
	ASSERT_EQ(expected_size, deserializer.getBufferSize(numEdges));
}

TEST(TestSimrankUpdateDeserializer, testNotEnough) {
	RandomWalk rw;
	rw.edge_num = 2;
	rw.fp_index = 5;
	rw.buf[0] = 1234;
	rw.buf[1] = 51;
	rw.buf[2] = 88888;

	char buf[1024];

	Serializer serializer;
	int s = 0;
	s += serializer.store(buf + s, rw.edge_num);
	s += serializer.store(buf + s, rw.buf[0]);
	s += serializer.store(buf + s, rw.buf[1]);

	SimrankUpdateDeserializer deserializer;
	ASSERT_FALSE(deserializer.checkReadable(buf, 10));
	ASSERT_TRUE(deserializer.checkReadable(buf, 28));

	ASSERT_FALSE(deserializer.checkReadable(buf, 0));
	ASSERT_FALSE(deserializer.checkReadable(buf, 1));
	ASSERT_FALSE(deserializer.checkReadable(buf, 2));
	ASSERT_FALSE(deserializer.checkReadable(buf, 3));

	ASSERT_EQ(-1, deserializer.storeFromBinary(buf, 0));
	ASSERT_EQ(-1, deserializer.storeFromBinary(buf, 1));
	ASSERT_EQ(-1, deserializer.storeFromBinary(buf, 2));
	ASSERT_EQ(-1, deserializer.storeFromBinary(buf, 3));
	ASSERT_EQ(-1, deserializer.storeFromBinary(buf, 10));
	ASSERT_EQ(-1, deserializer.storeFromBinary(buf, 27));
	ASSERT_EQ(28, deserializer.storeFromBinary(buf, 28));
	ASSERT_EQ(28, deserializer.storeFromBinary(buf, 29));
	ASSERT_EQ(28, deserializer.storeFromBinary(buf, 35));
	//ASSERT_EQ(expected_size, deserializer.getBufferSize(numEdges));
}

}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
