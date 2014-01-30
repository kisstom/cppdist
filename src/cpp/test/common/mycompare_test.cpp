/*
 * mycompare_test.cpp
 *
 *  Created on: 2013.07.26.
 *      Author: kisstom
 */


#include <gtest/gtest.h>
#include "../../main/common/util/MyCompare.h"
#include <map>
#include <queue>

//using std::string;
using std::vector;
using std::map;
using std::priority_queue;

namespace {

class MyCompareTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	MyCompareTest() {
  }

  virtual ~MyCompareTest() {
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


TEST(MyCompareTest, test1) {
	priority_queue<pair<long, double>, vector<pair<long, double> >, MyCompare> firstSortedScore;
	firstSortedScore.push(std::make_pair<long, double>(2, 1.0));
	firstSortedScore.push(std::make_pair<long, double>(1, 2.0));

	ASSERT_EQ(1, firstSortedScore.top().first);
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


