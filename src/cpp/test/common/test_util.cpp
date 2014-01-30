/*
 * test_util.cpp
 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <vector>
#include "../../main/common/util/util.h"
#include <string>

using std::vector;
using std::string;

namespace {

class TestUtil: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	TestUtil() {
  }

  virtual ~TestUtil() {
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

TEST(TestUtil, nextLong) {
  char line[1024] = "5 12 34 41 1\n";
  long path;
  size_t end = 2;
  end = Util::nextLong(line, end, path);
  ASSERT_EQ(12, path);

  end = Util::nextLong(line, end + 1, path);
  ASSERT_EQ(34, path);

  end = Util::nextLong(line, end + 1, path);
  ASSERT_EQ(41, path);

  end = Util::nextLong(line, end + 1, path);
  ASSERT_EQ(1, path);

  //ASSERT_EQ(string::npos, end);
}

TEST(TestUtil, nextLong2) {
	char line[1024] = "5 12 34";
  long path;
  size_t end = 2;
  end = Util::nextLong(line, 2, path);
  ASSERT_EQ(12, path);

  end = Util::nextLong(line, end + 1, path);
  ASSERT_EQ(34, path);

  //ASSERT_EQ(string::npos, end);
}

TEST(TestUtil, testReadEdges) {
	char line[1024] = "0 1 2 3\n";
	vector<long> edges;

	Util::readEdges(line, &edges);
	ASSERT_EQ(4, (int) edges.size());

	ASSERT_EQ(0, edges[0]);
	ASSERT_EQ(1, edges[1]);
	ASSERT_EQ(2, edges[2]);
  ASSERT_EQ(3, edges[3]);
}

TEST(TestUtil, testReadEdges2) {
	char line[1024] = "\n";
	vector<long> edges;

	Util::readEdges(line, &edges);
	ASSERT_EQ(0, edges.size());
}

TEST(TestUtil, testReadEdges3) {
	char line[1024] = "4\n";
	vector<long> edges;

	Util::readEdges(line, &edges);
	ASSERT_EQ(1, edges.size());
}

TEST(TestUtil, testSearch) {
	vector<long> longs;
	longs.push_back(0);
	longs.push_back(3);
	longs.push_back(4);
	longs.push_back(6);

	ASSERT_TRUE(Util::search(3, longs.begin() + 1, 2));
	ASSERT_FALSE(Util::search(3, longs.begin() + 2, 1));
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



