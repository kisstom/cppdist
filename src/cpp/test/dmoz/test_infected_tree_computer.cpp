/*
 * test_infected_tree_computer.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include "../../main/dmoz/infectedTreeComputer.h"

using std::vector;

namespace {

class InfectedTreeComputerTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  InfectedTreeComputerTest() {
  }

  virtual ~InfectedTreeComputerTest() {
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


TEST(InfectedTreeComputerTest, getRoot) {
  std::tr1::unordered_map<RootPath, std::set<long>, RootPathFunc, RootPathFunc> fpTreeNodes;
  std::tr1::unordered_map<int, std::set<long> > infectedNodes;

  std::set<long> root0;
  root0.insert(1);
  root0.insert(3);

  std::set<long> root1;
  root1.insert(2);
  root1.insert(3);

  fpTreeNodes[RootPath(0, 10)] = root0;
  fpTreeNodes[RootPath(1, 10)] = root1;

  std::set<long> infectedLev1;
  infectedLev1.insert(1);

  std::set<long> infectedLev2;
  infectedLev2.insert(2);

  infectedNodes[1] = infectedLev1;
  infectedNodes[2] = infectedLev2;

  InfectedTreeComputer computer(&fpTreeNodes, &infectedNodes);
  computer.computeInfectedTrees();

  ASSERT_EQ(1, computer.infectedTreeCount[1]);
  ASSERT_EQ(2, computer.infectedTreeCount[2]);
  //ASSERT_EQ(0, computer.infectedTreeCount[3]);

}

TEST(InfectedTreeComputerTest, testSmallMx) {
  std::tr1::unordered_map<RootPath, std::set<long>, RootPathFunc, RootPathFunc> fpTreeNodes;
  std::tr1::unordered_map<int, std::set<long> > infectedNodes;

  std::set<long> root0;
  root0.insert(2);

  std::set<long> root1;
  root1.insert(0);

  std::set<long> root2;
  root1.insert(5);

  fpTreeNodes[RootPath(2, 10)] = root0;
  fpTreeNodes[RootPath(3, 10)] = root1;
  fpTreeNodes[RootPath(20, 10)] = root2;

  std::set<long> infectedLev1;
  infectedLev1.insert(2);
  infectedLev1.insert(3);
  infectedLev1.insert(5);

  infectedNodes[1] = infectedLev1;

  InfectedTreeComputer computer(&fpTreeNodes, &infectedNodes);
  computer.computeInfectedTrees();

  ASSERT_EQ(2, computer.infectedTreeCount[1]);
}

TEST(InfectedTreeComputerTest, testBreakLevel) {
  std::tr1::unordered_map<RootPath, std::set<long>, RootPathFunc, RootPathFunc> fpTreeNodes;
  std::tr1::unordered_map<int, std::set<long> > infectedNodes;

  std::set<long> root0;
  root0.insert(1);
  root0.insert(2);

  fpTreeNodes[RootPath(0, 10)] = root0;

  std::set<long> infectedLev1;
  infectedLev1.insert(1);

  std::set<long> infectedLev2;
  infectedLev2.insert(2);

  infectedNodes[1] = infectedLev1;
  infectedNodes[2] = infectedLev2;

  InfectedTreeComputer computer(&fpTreeNodes, &infectedNodes);
  computer.computeInfectedTrees();

  ASSERT_EQ(1, computer.infectedTreeCount[1]);
  ASSERT_EQ(1, computer.infectedTreeCount[2]);
}


/*TEST(InfectedTreeComputerTest, getRootDiffSize) {
  std::tr1::unordered_map<RootPath, std::set<long>, RootPathFunc, RootPathFunc> fpTreeNodes;
  std::tr1::unordered_map<int, std::set<long> > infectedNodes;

  std::set<long> root0;
  root0.insert(1);
  root0.insert(3);

  std::set<long> root1;
  root1.insert(2);
  root1.insert(3);

  std::set<long> root2;
  root1.insert(2);
  root1.insert(3);

  fpTreeNodes[RootPath(0, 10)] = root0;
  fpTreeNodes[RootPath(1, 10)] = root1;
  fpTreeNodes[RootPath(1, 9)] = root2;

  std::set<long> infectedLev1;
  infectedLev1.insert(1);

  std::set<long> infectedLev2;
  infectedLev2.insert(2);

  infectedNodes[1] = infectedLev1;
  infectedNodes[2] = infectedLev2;

  InfectedTreeComputer computer(&fpTreeNodes, &infectedNodes);
  computer.computeInfectedTrees();

  ASSERT_EQ(1, computer.infectedTreeCount[1]);
  ASSERT_EQ(2, computer.infectedTreeCount[2]);
  //ASSERT_EQ(0, computer.infectedTreeCount[3]);

}*/

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}







