/*
 * test_simrank_score.cpp
 *
 *  Created on: 2014.07.08.
 *      Author: kisstom
 */

#include <gtest/gtest.h>

#include "../../main/dmoz/simrank.h"

namespace {
class SimrankScoreTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  SimrankScoreTest() {
  }

  virtual ~SimrankScoreTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  // Objects declared here can be used by all tests in the test case for Foo.

};

TEST(SimrankScoreTest, testScore) {
  SimRank sr(2, 10, 1, 0.9);
  unordered_map<long, vector<vector<long> > > pathes;

  vector<vector<long> > pathesfor1;
  vector<vector<long> > pathesfor2;

  vector<long> path1;
  path1.push_back(11247);
  path1.push_back(2980);
  path1.push_back(0);
  path1.push_back(1);
  path1.push_back(0);
  path1.push_back(1);
  path1.push_back(0);

  path1.push_back(1);
  path1.push_back(0);
  path1.push_back(1);
  path1.push_back(0);

  vector<long> path2;
  path2.push_back(365);
  path2.push_back(0);
  path2.push_back(1);
  path2.push_back(0);
  path2.push_back(1);
  path2.push_back(32);
  path2.push_back(0);

  path2.push_back(1);
  path2.push_back(0);
  path2.push_back(1);
  path2.push_back(0);

  pathesfor1.push_back(path1);
  pathesfor2.push_back(path2);
  pathes[365] = pathesfor1;
  pathes[11247] = pathesfor2;

  sr.setFringerPrint(pathes);
  double sc = sr.score(365, 11247);
  //ASSERT_NE(0.0, sc);
  ASSERT_NEAR(0.531, sc, 0.001);
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
