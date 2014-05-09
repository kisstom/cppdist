/*
 * test_edgelist_container.cpp
 *
 *  Created on: 2013.07.30.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../../main/common/graph/edgelist_container.h"

using std::vector;
using std::string;

namespace {

class EdgelistContainerTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	EdgelistContainerTest() {
  }

  virtual ~EdgelistContainerTest() {
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

TEST(EdgelistContainerTest, testInitContainer) {
	EdgelistContainer container;
	ASSERT_FALSE(container.shouldDeleteContainers());
	container.initContainers();
	container.setFinish();

	ASSERT_EQ(0, container.getNumberOfNodes());
	ASSERT_EQ(0, container.getNumberOfEdges());
	ASSERT_EQ(-1, container.getMinnode());

	ASSERT_TRUE(container.shouldDeleteContainers());
}

TEST(EdgelistContainerTest, testAddOne) {
	EdgelistContainer container;
	container.setMinnode(5);
	container.initContainers();

	container.addEdge(5, 3);
	container.setFinish();
	ASSERT_EQ(1, container.getNumberOfNodes());
	ASSERT_EQ(1, container.getNumberOfEdges());

	ASSERT_EQ(1, container.neighborhoodSize(5));
}

TEST(EdgelistContainerTest, testAdd) {
	EdgelistContainer container;
	container.setMinnode(5);
	container.initContainers();

	container.addEdge(7, 3);
	container.addEdge(7, 5);
	container.addEdge(9, 1);
	container.addEdge(10, 2);
	container.setFinish();

	ASSERT_EQ(6, container.getNumberOfNodes());
	ASSERT_EQ(4, container.getNumberOfEdges());

	ASSERT_EQ(2, container.neighborhoodSize(7));
	ASSERT_EQ(0, container.neighborhoodSize(8));
	ASSERT_EQ(1, container.neighborhoodSize(10));
}

TEST(EdgelistContainerTest, testEmpty) {
	EdgelistContainer container;
	container.setMinnode(0);
	container.initContainers();
	container.setFinish();

	ASSERT_EQ(-1, container.neighborhoodSize(0));
}

TEST(EdgelistContainerTest, testInit) {
	EdgelistContainer container;
	ASSERT_FALSE(container.shouldDeleteContainers());
	container.initContainers();
	ASSERT_TRUE(container.shouldDeleteContainers());
}

TEST(EdgelistContainerTest, testArrayOut) {
	EdgelistContainer container;
	container.setMinnode(1);
	container.initContainers();
  container.addEdge(2, 0);
	container.setFinish();

	ASSERT_EQ(1, container.neighborhoodSize(2));
	ASSERT_EQ(-1, container.neighborhoodSize(3));
}

TEST(EdgelistContainerTest, testContainsEdge) {
	EdgelistContainer container;
	container.setMinnode(0);
	container.initContainers();
  container.addEdge(2, 0);
  container.addEdge(2, 1);
  container.addEdge(2, 3);
	container.setFinish();

	ASSERT_FALSE(container.containsEdge(3, 0));
	ASSERT_FALSE(container.containsEdge(1, 0));
	ASSERT_TRUE(container.containsEdge(2, 1));
	ASSERT_FALSE(container.containsEdge(2, 4));
}

TEST(EdgelistContainerTest, testEdgeAtPos) {
	EdgelistContainer container;
	container.setMinnode(1);
	container.initContainers();
	container.addEdge(3, 0);
	container.addEdge(3, 1);
	container.addEdge(3, 3);
	container.setFinish();

	ASSERT_EQ(1, container.getEdgeAtPos(3, 1));
	ASSERT_EQ(3, container.getEdgeAtPos(3, 2));
}

TEST(EdgelistContainerTest, testEquals) {
	EdgelistContainer container;
	container.setMinnode(1);
	container.initContainers();
	container.addEdge(3, 0);
	container.addEdge(3, 1);
	container.addEdge(3, 3);
	container.setFinish();

	EdgelistContainer container2;
	container2.setMinnode(1);
	container2.initContainers();
	container2.addEdge(3, 0);
	container2.addEdge(3, 1);
	container2.addEdge(3, 3);
	container2.setFinish();

	ASSERT_TRUE(container == container2);
	ASSERT_TRUE(container == container);
}

TEST(EdgelistContainerTest, testDiffMinnode) {
	EdgelistContainer container;
	container.setMinnode(3);
	container.initContainers();
	container.addEdge(3, 0);
	container.addEdge(3, 1);
	container.addEdge(3, 3);
	container.setFinish();

	EdgelistContainer container2;
	container2.setMinnode(1);
	container2.initContainers();
	container2.addEdge(3, 0);
	container2.addEdge(3, 1);
	container2.addEdge(3, 3);
	container2.setFinish();

	ASSERT_FALSE(container == container2);
}

TEST(EdgelistContainerTest, testDiffEdges) {
	EdgelistContainer container;
	container.setMinnode(3);
	container.initContainers();
	container.addEdge(3, 0);
	container.addEdge(3, 2);
	container.addEdge(3, 3);
	container.setFinish();

	EdgelistContainer container2;
	container2.setMinnode(1);
	container2.initContainers();
	container2.addEdge(3, 0);
	container2.addEdge(3, 1);
	container2.addEdge(3, 3);
	container2.setFinish();

	ASSERT_FALSE(container == container2);
}


}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



