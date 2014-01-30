/*
 * simrank_store_first_deserializer_test.cpp
 *
 *  Created on: 2013.11.07.
 *      Author: kisstom
 */

#include <gtest/gtest.h>

#include "../../../main/algos/simrank_store_first/simrank_store_first_deserializer.h"

namespace {

class SimrankStoreFirstDeserializerTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SimrankStoreFirstDeserializerTest() {
  }

  virtual ~SimrankStoreFirstDeserializerTest() {
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

TEST(SimrankStoreFirstDeserializerTest, test) {}
  SimrankStoreFirstDeserializer deserializer;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



