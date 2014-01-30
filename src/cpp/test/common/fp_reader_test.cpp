/*
 * fp_reader_test.cpp
 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <vector>
#include "../../main/common/components/fingerprint_reader.h"
#include <string>

using std::vector;
using std::string;

namespace {

class FingerprintReaderTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	FingerprintReaderTest() {
  }

  virtual ~FingerprintReaderTest() {
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


TEST(FingerprintReaderTest, test1) {
	FingerprintReader fpReader;
	list<RandomWalk> randomWalks;
  fpReader.setRWContainer(&randomWalks);

  char line[1024] = "5 12 34 41 1\n";
  RandomWalk randomWalk;

  fpReader.readWalk(line, randomWalk, 4, 20);
  ASSERT_EQ(1, (int) randomWalks.size());

  ASSERT_EQ(5, randomWalks.front().fp_index);
  ASSERT_EQ(12, randomWalks.front().buf[0]);
  ASSERT_EQ(34, randomWalks.front().buf[1]);
  ASSERT_EQ(41, randomWalks.front().buf[2]);
  ASSERT_EQ(1, randomWalks.front().buf[3]);
  ASSERT_EQ(3, randomWalks.front().edge_num);
}

TEST(FingerprintReaderTest, testOutOfBounds) {
	FingerprintReader fpReader;
	list<RandomWalk> randomWalks;
  fpReader.setRWContainer(&randomWalks);

  char line[1024] = "5 12 34 41 1\n";
  RandomWalk randomWalk;

  fpReader.readWalk(line, randomWalk, 4, 12);
  ASSERT_EQ(0, randomWalks.size());

  fpReader.readWalk(line, randomWalk, 13, 19);
  ASSERT_EQ(0, randomWalks.size());
}

TEST(FingerprintReaderTest, test_empty) {
	FingerprintReader fpReader;
	list<RandomWalk> randomWalks;
  fpReader.setRWContainer(&randomWalks);

  char line[1024] = "5\n";
  RandomWalk randomWalk;

  fpReader.readWalk(line, randomWalk, 2, 7);
  ASSERT_EQ(0, randomWalks.size());
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


