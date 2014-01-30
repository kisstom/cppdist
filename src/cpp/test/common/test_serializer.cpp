/*
 * test_serializer.cpp
 *
 *  Created on: 2013.08.08.
 *      Author: kisstom
 */

#include "../../main/common/components/serializer.h"
#include <gtest/gtest.h>

namespace {

class SerializerTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SerializerTest() {
  }

  virtual ~SerializerTest() {
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
}

TEST(SerializerTest, test) {
  Serializer serializer;
  char a[1024];

  long longToStore1 = 1234;
  long longToStore2 = 12341;
  long readLong;
  int s = serializer.store(a, longToStore1);
  s += serializer.store(a + s, longToStore2);
  s += serializer.setBreak(a + s);
  s += serializer.store(a + s, longToStore2);
  s += serializer.store(a + s, longToStore1);
  serializer.setFinish(a + s);

  //s = serializer.read(a, &readLong, sizeof(long));
  s = 0;
  memcpy(&readLong, a, sizeof(long));
  s += sizeof(long);
  ASSERT_EQ(longToStore1, readLong);

  memcpy(&readLong, a + s, sizeof(long));
  s += sizeof(long);
  ASSERT_EQ(longToStore2, readLong);

  ASSERT_TRUE(serializer.hasNext(a + s));
  s += 1;

  memcpy(&readLong, a + s, sizeof(long));
  s += sizeof(long);
  ASSERT_EQ(longToStore2, readLong);

  memcpy(&readLong, a + s, sizeof(long));
  s += sizeof(long);
  ASSERT_EQ(longToStore1, readLong);

  ASSERT_FALSE(serializer.hasNext(a + s));
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


