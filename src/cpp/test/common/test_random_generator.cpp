/*
 * test_random_generator.cpp
 *
 *  Created on: 2013.08.29.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <vector>
#include "../../main/common/random/random_generator.h"
#include <string>

using std::vector;
using std::string;

namespace {

class TestRandomGenerator: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	TestRandomGenerator() {
  }

  virtual ~TestRandomGenerator() {
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


TEST(TestRandomGenerator, showRandoms) {
  RandomGenerator generator(13);
  //generator.setSeed(13);

  double rnm = generator.uniRand(1);
  ASSERT_NEAR(0.632643, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.341416, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.904110, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.130035, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.608363, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.334507, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.155829, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.794827, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.928201, rnm, 0.0001);

  rnm = generator.uniRand(1);
  ASSERT_NEAR(0.556065, rnm, 0.0001);
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
