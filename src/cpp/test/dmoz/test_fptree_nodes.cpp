/*
 * test_fptree_nodes.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */


#include <gtest/gtest.h>
#include "../../main/dmoz/fpTreeLeaves.h"

using std::vector;

namespace {

class FPTreeNodesTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FPTreeNodesTest() {
  }

  virtual ~FPTreeNodesTest() {
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


TEST(FPTreeNodesTest, addInner) {
  char line[] = "0 1 2 3 4 5";
  FpTreeLeaves fpTreeLeaves;
  fpTreeLeaves.addInnerNodes(line);

  RootPath rp(5, 4);
  ASSERT_FALSE(fpTreeLeaves.nodes.find(rp) == fpTreeLeaves.nodes.end());

  ASSERT_TRUE(fpTreeLeaves.nodes[rp].find(0) == fpTreeLeaves.nodes[rp].end());
  ASSERT_FALSE(fpTreeLeaves.nodes[rp].find(1) == fpTreeLeaves.nodes[rp].end());
  ASSERT_FALSE(fpTreeLeaves.nodes[rp].find(2) == fpTreeLeaves.nodes[rp].end());
  ASSERT_FALSE(fpTreeLeaves.nodes[rp].find(3) == fpTreeLeaves.nodes[rp].end());
  ASSERT_FALSE(fpTreeLeaves.nodes[rp].find(4) == fpTreeLeaves.nodes[rp].end());
  ASSERT_EQ(1, fpTreeLeaves.nodes.size());
  ASSERT_EQ(4, fpTreeLeaves.nodes[rp].size());
}

TEST(FPTreeNodesTest, addSink) {
  char line[] = "0 5";

  FpTreeLeaves fpTreeLeaves;
  fpTreeLeaves.addInnerNodes(line);
  ASSERT_EQ(0, fpTreeLeaves.nodes.size());
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

