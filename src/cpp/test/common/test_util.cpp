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

TEST(TestUtil, testCheckParams) {
  unordered_map<string, string> params;
  params["par1"] = "foo1";
  params["par2"] = "foo2";

  Util util;
  bool found = false;
  try {
    util.checkParam(&params, 3, "par1", "par2", "par3");
  } catch (ParamMissException &) {
    found = true;
  }

  ASSERT_TRUE(found);
}

TEST(TestUtil, testCheckParams2) {
  unordered_map<string, string> params;
  params["par1"] = "foo1";
  params["par2"] = "foo2";

  Util util;
  bool found = false;
  try {
    util.checkParam(&params, 2, "par3", "par1");
  } catch (ParamMissException &) {
    found = true;
  }

  ASSERT_TRUE(found);
}

TEST(TestUtil, testCheckParams3) {
  unordered_map<string, string> params;
  params["par1"] = "foo1";
  params["par2"] = "foo2";

  Util util;
  bool found = false;
  try {
    util.checkParam(&params, 2, "par2", "par1");
  } catch (ParamMissException &) {
    found = true;
  }

  ASSERT_FALSE(found);
}

TEST(TestUtil, testSplit) {
  Util util;

  vector<string> spl = util.split("0,1,2,3,4", ',');
  ASSERT_EQ(0, spl[0].compare("0"));
  ASSERT_EQ(0, spl[1].compare("1"));
  ASSERT_EQ(0, spl[2].compare("2"));
  ASSERT_EQ(0, spl[3].compare("3"));
  ASSERT_EQ(0, spl[4].compare("4"));
}

TEST(TestUtil, testConvert) {
  Util util;

  vector<string> str;
  str.push_back("0");
  str.push_back("1");
  str.push_back("2");
  str.push_back("3");
  str.push_back("4");

  vector<long> longs = util.convertToLong(str);
  ASSERT_EQ(0, longs[0]);
  ASSERT_EQ(1, longs[1]);
  ASSERT_EQ(2, longs[2]);
  ASSERT_EQ(3, longs[3]);
  ASSERT_EQ(4, longs[4]);

}

TEST(TestUtil, testHasSection) {
  std::set<long> a;
  a.insert(1);
  a.insert(2);
  a.insert(3);

  std::set<long> b;
  b.insert(4);
  b.insert(5);
  b.insert(3);

  std::set<long> c;
  b.insert(7);
  b.insert(5);
  b.insert(4);

  Util util;
  ASSERT_TRUE(util.hasSection(a, b));
  ASSERT_FALSE(util.hasSection(a, c));
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



