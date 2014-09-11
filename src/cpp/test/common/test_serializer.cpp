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

// Tests serializing long vars and placing separators.
TEST(SerializerTest, test) {
  Serializer serializer;
  char a[1024];

  // Sets up variables to serialize.
  long longToStore1 = 1234;
  long longToStore2 = 12341;
  long readLong;

  // Serializes vars.
  int s = serializer.store(a, longToStore1);
  s += serializer.store(a + s, longToStore2);
  s += serializer.setBreak(a + s);
  s += serializer.store(a + s, longToStore2);
  s += serializer.store(a + s, longToStore1);
  serializer.setFinish(a + s);

  // Reads back and checks.
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

// Tests serializing and reading back bytes or unsigned bytes.
TEST(SerializerTest, testBytes) {
  Serializer serializer;
  unsigned char ser[1024];
  char buffer[1024];

  ser[0] = 20;
  ser[1] = 40;
  ser[2] = 60;
  ser[3] = 80;
  ser[4] = 100;
  ser[5] = 160;

  serializer.store(buffer, ser, 2);
  serializer.store(buffer + 2, ser + 2, 2);
  serializer.store(buffer + 4, ser + 4, 2);

  ASSERT_EQ(20, buffer[0]);
  ASSERT_EQ(40, buffer[1]);
  ASSERT_EQ(60, buffer[2]);
  ASSERT_EQ(80, buffer[3]);
  ASSERT_EQ(100, buffer[4]);
  ASSERT_EQ(160, (unsigned char) buffer[5]);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


