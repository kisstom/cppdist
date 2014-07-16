/*
 * test_infected_node_computer.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include "../../main/dmoz/infectedNodeComputer.h"

using std::vector;

namespace {

class InfectedNodeComputerTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  InfectedNodeComputerTest() {
  }

  virtual ~InfectedNodeComputerTest() {
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


TEST(InfectedNodeComputerTest, getRoot) {
  vector<long> crawlMaxes;
  crawlMaxes.push_back(10);
  crawlMaxes.push_back(20);
  crawlMaxes.push_back(30);

  InfectedNodeComputer computer(crawlMaxes);
  ASSERT_EQ(0, computer.findInfectedIndex(2));
  ASSERT_EQ(0, computer.findInfectedIndex(10));
  ASSERT_EQ(1, computer.findInfectedIndex(12));
  ASSERT_EQ(1, computer.findInfectedIndex(20));
  ASSERT_EQ(2, computer.findInfectedIndex(22));
  ASSERT_EQ(2, computer.findInfectedIndex(30));
  ASSERT_EQ(-1, computer.findInfectedIndex(31));
}

TEST(InfectedNodeComputerTest, addInfectedNodes) {
  vector<long> crawlMaxes;
  crawlMaxes.push_back(10);
  crawlMaxes.push_back(20);
  crawlMaxes.push_back(30);

  InfectedNodeComputer computer(crawlMaxes);

  vector<long> edges;
  edges.push_back(2);
  edges.push_back(10);

  computer.addInfectedNodes(edges);

  ASSERT_EQ(0, computer.infectedNodes.size());
}

TEST(InfectedNodeComputerTest, addInfectedNodes2) {
  vector<long> crawlMaxes;
  crawlMaxes.push_back(10);
  crawlMaxes.push_back(20);
  crawlMaxes.push_back(30);

  InfectedNodeComputer computer(crawlMaxes);

  vector<long> edges;
  edges.push_back(2);
  edges.push_back(10);
  edges.push_back(12);
  edges.push_back(20);

  computer.addInfectedNodes(edges);
  ASSERT_FALSE(computer.infectedNodes[1].find(0) == computer.infectedNodes[1].end());
}

TEST(InfectedNodeComputerTest, addInfectedNodes3) {
  vector<long> crawlMaxes;
  crawlMaxes.push_back(10);
  crawlMaxes.push_back(20);
  crawlMaxes.push_back(30);

  InfectedNodeComputer computer(crawlMaxes);

  vector<long> edges;
  edges.push_back(2);
  edges.push_back(10);
  edges.push_back(22);
  edges.push_back(30);

  computer.addInfectedNodes(edges);
  ASSERT_TRUE(computer.infectedNodes[1].find(0) == computer.infectedNodes[1].end());
  ASSERT_FALSE(computer.infectedNodes[2].find(0) == computer.infectedNodes[2].end());
  ASSERT_TRUE(computer.infectedNodes[3].find(0) == computer.infectedNodes[3].end());
}

TEST(InfectedNodeComputerTest, addInfectedNodes4) {
  vector<long> crawlMaxes;
  crawlMaxes.push_back(10);
  crawlMaxes.push_back(20);
  crawlMaxes.push_back(30);

  InfectedNodeComputer computer(crawlMaxes);

  vector<long> edges;
  edges.push_back(2);
  edges.push_back(10);
  edges.push_back(31);

  computer.addInfectedNodes(edges);
  ASSERT_EQ(0, computer.infectedNodes.size());
  ASSERT_TRUE(computer.infectedNodes[1].find(0) == computer.infectedNodes[1].end());
  ASSERT_TRUE(computer.infectedNodes[2].find(0) == computer.infectedNodes[2].end());
  ASSERT_TRUE(computer.infectedNodes[3].find(0) == computer.infectedNodes[3].end());
}

TEST(InfectedNodeComputerTest, testRedSmallMx) {
  vector<long> crawlMaxes;
  crawlMaxes.push_back(5);
  crawlMaxes.push_back(12);
  InfectedNodeComputer computer(crawlMaxes);

  vector<long> edges;
  edges.push_back(1);
  edges.push_back(2);
  edges.push_back(3);
  computer.addInfectedNodes(edges);
  computer.incrementNumLine();

  edges.clear();
  computer.addInfectedNodes(edges);
  computer.incrementNumLine();

  edges.clear();
  edges.push_back(9);
  computer.addInfectedNodes(edges);
  computer.incrementNumLine();

  edges.clear();
  edges.push_back(8);
  edges.push_back(10);
  computer.addInfectedNodes(edges);
  computer.incrementNumLine();

  edges.clear();
  computer.addInfectedNodes(edges);
  computer.incrementNumLine();

  edges.clear();
  edges.push_back(2);
  edges.push_back(4);
  edges.push_back(6);
  computer.addInfectedNodes(edges);
  computer.incrementNumLine();

  ASSERT_FALSE(computer.infectedNodes[1].find(2) == computer.infectedNodes[1].end());
  ASSERT_FALSE(computer.infectedNodes[1].find(3) == computer.infectedNodes[1].end());
  ASSERT_FALSE(computer.infectedNodes[1].find(5) == computer.infectedNodes[1].end());
  ASSERT_EQ(3, computer.infectedNodes[1].size());
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}




