/*
 * split_by_row_test.cpp
 *
 *  Created on: 2013.12.11.
 *      Author: kisstom
 */
#include "../../../main/common/graph_converter/split_by_row.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream>

using std::vector;
using std::stringstream;
using std::ios_base;
using std::stringstream;

namespace {

class SplitByRowTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SplitByRowTest() {
  }

  virtual ~SplitByRowTest() {
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

TEST(SplitByRowTest, testEdgeCount) {
	SplitByRow splitByRow;
	char line[1024] = "1 2 3 4\n";

	long count = splitByRow.countEdges(line);
	ASSERT_EQ(4, count);

	char line2[1024] = "1 2 3 4";

	count = splitByRow.countEdges(line2);
  ASSERT_EQ(4, count);

  char line3[1024] = "\n";
	count = splitByRow.countEdges(line3);
  ASSERT_EQ(0, count);

  char line4[1024] = "";
  count = splitByRow.countEdges(line4);
  ASSERT_EQ(0, count);
}

}
int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

