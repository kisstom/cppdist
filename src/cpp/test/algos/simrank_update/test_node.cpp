/*
 * test_node.cpp
 *
 *  Created on: 2013.07.30.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "../../../main/algos/simrank_update/simrank_update_node.h"
#include "../../../main/common/components/sender_buffer.h"
#include "../../common/mock_socket_manager.h"

using std::vector;
using std::string;

namespace {

class SimrankUpdateNodeTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SimrankUpdateNodeTest() {
  }

  virtual ~SimrankUpdateNodeTest() {
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

TEST(SimrankUpdateNodeTest, testGenRandomFromOneCont) {
  SimrankUpdateNode node;
  SortedVectorNodeMatrix matrix;
  matrix.initEdgelistContainers();
  matrix.getNewlyAddedMatrix()->setMinnode(0);

  vector<long> start_edge_upd;
  vector<long> edge_list_upd;
  start_edge_upd.push_back(0);
  start_edge_upd.push_back(1);
  start_edge_upd.push_back(1);
  edge_list_upd.push_back(2);

  matrix.getNewlyAddedMatrix()->setEdgeList(&edge_list_upd);
  matrix.getNewlyAddedMatrix()->setStartEdges(&start_edge_upd);

  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand
  ASSERT_EQ(2, node.generateRandomNeighbour(0, matrix.getNewlyAddedMatrix()));
  ASSERT_EQ(-1, node.generateRandomNeighbour(1, matrix.getNewlyAddedMatrix()));
  ASSERT_EQ(-1, node.generateRandomNeighbour(3, matrix.getNewlyAddedMatrix()));
}

TEST(SimrankUpdateNodeTest, testGenRandomFromOneCont2) {
	SimrankUpdateNode node;
  SortedVectorNodeMatrix matrix;
  matrix.initEdgelistContainers();
  matrix.getNewlyAddedMatrix()->setMinnode(0);

  vector<long> start_edge_upd;
  vector<long> edge_list_upd;
  start_edge_upd.push_back(0);
  start_edge_upd.push_back(2);
  edge_list_upd.push_back(2);
  edge_list_upd.push_back(3);

  matrix.getNewlyAddedMatrix()->setEdgeList(&edge_list_upd);
  matrix.getNewlyAddedMatrix()->setStartEdges(&start_edge_upd);

  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand
  ASSERT_EQ(3, node.generateRandomNeighbour(0, matrix.getNewlyAddedMatrix()));
}

TEST(SimrankUpdateNodeTest, testGenRandomFromTwoContOutFirst) {
	SimrankUpdateNode node;
  EdgelistContainer* cont1 = new EdgelistContainer();
  cont1->setMinnode(2);
  EdgelistContainer* cont2 = new EdgelistContainer();
  cont2->setMinnode(2);

  vector<long> start_edge_upd1;
  vector<long> edge_list_upd1;
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(2);
  start_edge_upd1.push_back(2);
  edge_list_upd1.push_back(2);
  edge_list_upd1.push_back(3);

  cont1->setEdgeList(&edge_list_upd1);
  cont1->setStartEdges(&start_edge_upd1);

  vector<long> start_edge_upd2;
  vector<long> edge_list_upd2;
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(3);
  start_edge_upd2.push_back(3);
  start_edge_upd2.push_back(4);
  edge_list_upd2.push_back(4);
  edge_list_upd2.push_back(5);
  edge_list_upd2.push_back(6);
  edge_list_upd2.push_back(7);

  cont2->setEdgeList(&edge_list_upd2);
  cont2->setStartEdges(&start_edge_upd2);

  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  ASSERT_EQ(5, node.generateRandomNeighbour(2,cont1, cont2));
  ASSERT_EQ(3, node.generateRandomNeighbour(2,cont1, cont2));
  ASSERT_EQ(-1, node.generateRandomNeighbour(3,cont1, cont2));
  ASSERT_EQ(7, node.generateRandomNeighbour(4,cont1, cont2));
}

TEST(SimrankUpdateNodeTest, testGenRandomFromTwoContOutSecond) {
	SimrankUpdateNode node;
  EdgelistContainer* cont1 = new EdgelistContainer();
  cont1->setMinnode(2);
  EdgelistContainer* cont2 = new EdgelistContainer();
  cont2->setMinnode(2);

  vector<long> start_edge_upd1;
  vector<long> edge_list_upd1;
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(2);
  start_edge_upd1.push_back(2);
  start_edge_upd1.push_back(2);
  start_edge_upd1.push_back(3);
  edge_list_upd1.push_back(2);
  edge_list_upd1.push_back(3);
  edge_list_upd1.push_back(8);

  cont1->setEdgeList(&edge_list_upd1);
  cont1->setStartEdges(&start_edge_upd1);

  vector<long> start_edge_upd2;
  vector<long> edge_list_upd2;
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(3);
  start_edge_upd2.push_back(3);

  edge_list_upd2.push_back(4);
  edge_list_upd2.push_back(5);
  edge_list_upd2.push_back(6);

  cont2->setEdgeList(&edge_list_upd2);
  cont2->setStartEdges(&start_edge_upd2);

  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  ASSERT_EQ(5, node.generateRandomNeighbour(2,cont1, cont2));
  ASSERT_EQ(3, node.generateRandomNeighbour(2,cont1, cont2));
  ASSERT_EQ(-1, node.generateRandomNeighbour(4,cont1, cont2));
  ASSERT_EQ(8, node.generateRandomNeighbour(5,cont1, cont2));
}

TEST(SimrankUpdateNodeTest, testGenRandomFromTwoContEmptyFirst) {
	SimrankUpdateNode node;
  EdgelistContainer* cont1 = new EdgelistContainer();
  cont1->setMinnode(2);
  EdgelistContainer* cont2 = new EdgelistContainer();
  cont2->setMinnode(2);

  vector<long> start_edge_upd1;
  vector<long> edge_list_upd1;
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(2);
  edge_list_upd1.push_back(2);
  edge_list_upd1.push_back(3);

  cont1->setEdgeList(&edge_list_upd1);
  cont1->setStartEdges(&start_edge_upd1);

  vector<long> start_edge_upd2;
  vector<long> edge_list_upd2;
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(3);
  start_edge_upd2.push_back(3);
  edge_list_upd2.push_back(4);
  edge_list_upd2.push_back(5);
  edge_list_upd2.push_back(6);

  cont2->setEdgeList(&edge_list_upd2);
  cont2->setStartEdges(&start_edge_upd2);

  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  ASSERT_EQ(5, node.generateRandomNeighbour(2,cont1, cont2));
}

TEST(SimrankUpdateNodeTest, testGenRandomFromTwoContEmptySecond) {
	SimrankUpdateNode node;
  EdgelistContainer* cont1 = new EdgelistContainer();
  cont1->setMinnode(2);
  EdgelistContainer* cont2 = new EdgelistContainer();
  cont2->setMinnode(2);

  vector<long> start_edge_upd1;
  vector<long> edge_list_upd1;
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(2);
  start_edge_upd1.push_back(2);
  edge_list_upd1.push_back(2);
  edge_list_upd1.push_back(3);

  cont1->setEdgeList(&edge_list_upd1);
  cont1->setStartEdges(&start_edge_upd1);

  vector<long> start_edge_upd2;
  vector<long> edge_list_upd2;
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(3);
  edge_list_upd2.push_back(4);
  edge_list_upd2.push_back(5);
  edge_list_upd2.push_back(6);

  cont2->setEdgeList(&edge_list_upd2);
  cont2->setStartEdges(&start_edge_upd2);

  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  ASSERT_EQ(3, node.generateRandomNeighbour(2,cont1, cont2));
}


TEST(SimrankUpdateNodeTest, flipCoinToStay) {
	SimrankUpdateNode node;
  SortedVectorNodeMatrix matrix;
  matrix.initEdgelistContainers();
  matrix.getNewlyAddedMatrix()->setMinnode(3);
  matrix.getUnchangedMatrix()->setMinnode(3);

  vector<long> start_edge_upd1;
  vector<long> edge_list_upd1;
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(2);
  edge_list_upd1.push_back(2);
  edge_list_upd1.push_back(3);

  matrix.getUnchangedMatrix()->setEdgeList(&edge_list_upd1);
  matrix.getUnchangedMatrix()->setStartEdges(&start_edge_upd1);

  vector<long> start_edge_upd2;
  vector<long> edge_list_upd2;
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(3);
  edge_list_upd2.push_back(4);
  edge_list_upd2.push_back(5);
  edge_list_upd2.push_back(6);

  matrix.getNewlyAddedMatrix()->setEdgeList(&edge_list_upd2);
  matrix.getNewlyAddedMatrix()->setStartEdges(&start_edge_upd2);

  node.setMatrix(&matrix);
  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  ASSERT_FALSE(node.flipCoinToStay(3));
  ASSERT_TRUE(node.flipCoinToStay(3));
  //ASSERT_EQ(3, node.generateRandomNeighbour(3, cont1, cont2));
}

TEST(SimrankUpdateNodeTest, flipCoinToStayEmptyFirst) {
	SimrankUpdateNode node;
  SortedVectorNodeMatrix matrix;
  matrix.initEdgelistContainers();
  matrix.getNewlyAddedMatrix()->setMinnode(3);
  matrix.getUnchangedMatrix()->setMinnode(3);

  vector<long> start_edge_upd1;
  vector<long> edge_list_upd1;
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(0);

  matrix.getUnchangedMatrix()->setEdgeList(&edge_list_upd1);
  matrix.getUnchangedMatrix()->setStartEdges(&start_edge_upd1);

  vector<long> start_edge_upd2;
  vector<long> edge_list_upd2;
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(3);
  edge_list_upd2.push_back(4);
  edge_list_upd2.push_back(5);
  edge_list_upd2.push_back(6);

  matrix.getNewlyAddedMatrix()->setEdgeList(&edge_list_upd2);
  matrix.getNewlyAddedMatrix()->setStartEdges(&start_edge_upd2);

  node.setMatrix(&matrix);
  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  ASSERT_FALSE(node.flipCoinToStay(3));
  ASSERT_FALSE(node.flipCoinToStay(3));
  //ASSERT_EQ(3, node.generateRandomNeighbour(3, cont1, cont2));
}

TEST(SimrankUpdateNodeTest, flipCoinToStayEmptySecond) {
	SimrankUpdateNode node;
  SortedVectorNodeMatrix matrix;
  matrix.initEdgelistContainers();
  matrix.getNewlyAddedMatrix()->setMinnode(3);
  matrix.getUnchangedMatrix()->setMinnode(3);

  vector<long> start_edge_upd1;
  vector<long> edge_list_upd1;
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(2);
  edge_list_upd1.push_back(2);
  edge_list_upd1.push_back(3);

  matrix.getUnchangedMatrix()->setEdgeList(&edge_list_upd1);
  matrix.getUnchangedMatrix()->setStartEdges(&start_edge_upd1);

  vector<long> start_edge_upd2;
  vector<long> edge_list_upd2;
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(0);

  matrix.getNewlyAddedMatrix()->setEdgeList(&edge_list_upd2);
  matrix.getNewlyAddedMatrix()->setStartEdges(&start_edge_upd2);

  node.setMatrix(&matrix);
  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  ASSERT_TRUE(node.flipCoinToStay(3));
  ASSERT_TRUE(node.flipCoinToStay(3));
  //ASSERT_EQ(3, node.generateRandomNeighbour(3, cont1, cont2));
}

TEST(SimrankUpdateNodeTest, flipCoinToStayOut) {
	SimrankUpdateNode node;
  SortedVectorNodeMatrix matrix;
  matrix.initEdgelistContainers();
  matrix.getNewlyAddedMatrix()->setMinnode(3);
  matrix.getUnchangedMatrix()->setMinnode(3);

  vector<long> start_edge_upd1;
  vector<long> edge_list_upd1;
  start_edge_upd1.push_back(0);
  start_edge_upd1.push_back(2);
  edge_list_upd1.push_back(2);
  edge_list_upd1.push_back(3);

  matrix.getUnchangedMatrix()->setEdgeList(&edge_list_upd1);
  matrix.getUnchangedMatrix()->setStartEdges(&start_edge_upd1);

  vector<long> start_edge_upd2;
  vector<long> edge_list_upd2;
  start_edge_upd2.push_back(0);
  start_edge_upd2.push_back(0);

  matrix.getNewlyAddedMatrix()->setEdgeList(&edge_list_upd2);
  matrix.getNewlyAddedMatrix()->setStartEdges(&start_edge_upd2);

  node.setMatrix(&matrix);
  node.setSeed(13); // implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  ASSERT_TRUE(node.flipCoinToStay(4));
}

TEST(SimrankUpdateNodeTest, serializeToSender) {
	SimrankUpdateNode node;
  SenderBuffer buffer;
  MockSocketManager * socketManager = new MockSocketManager;
  buffer.setSocketManager(socketManager);

  buffer.resizeBufferNum(1);
  buffer.resizeBuffers(60);
  node.setSenderBuffer(&buffer);

  RandomWalk rw;
  rw.edge_num = 2;
  rw.buf[0] = 1;
  rw.buf[1] = 2;
  rw.buf[2] = 3;
  rw.fp_index = 2;

  EXPECT_CALL(*socketManager, sendToNode(58, buffer.pack_[0], 0));

  node.serializeToSender(rw, 0);
  ASSERT_EQ(29, buffer.getBufferSize(0));

  node.serializeToSender(rw, 0);
  ASSERT_EQ(58, buffer.getBufferSize(0));

  node.serializeToSender(rw, 0);
  ASSERT_EQ(29, buffer.getBufferSize(0));

  //node.addEndSignal();
  //ASSERT_EQ(30, buffer.getBufferSize(0));

  delete socketManager;
}

TEST(SimrankUpdateNodeTest, testNextEdge) {
	SimrankUpdateNode node;
	SortedVectorNodeMatrix matrix;
	matrix.initEdgelistContainers();
	matrix.getNewlyAddedMatrix()->setMinnode(3);
	matrix.getUnchangedMatrix()->setMinnode(3);
	matrix.getDeletedMatrix()->setMinnode(3);

	vector<long> start_edge_upd1;
	vector<long> edge_list_upd1;
	start_edge_upd1.push_back(0);
	start_edge_upd1.push_back(2);
	start_edge_upd1.push_back(2);
	start_edge_upd1.push_back(3);
	edge_list_upd1.push_back(3);
	edge_list_upd1.push_back(4);
	edge_list_upd1.push_back(5);

	matrix.getUnchangedMatrix()->setEdgeList(&edge_list_upd1);
	matrix.getUnchangedMatrix()->setStartEdges(&start_edge_upd1);

	vector<long> start_edge_upd2;
	vector<long> edge_list_upd2;
	start_edge_upd2.push_back(0);
	start_edge_upd2.push_back(1);
	start_edge_upd2.push_back(3);
	start_edge_upd2.push_back(3);
	edge_list_upd2.push_back(5);
	edge_list_upd2.push_back(4);
	edge_list_upd2.push_back(3);

	matrix.getNewlyAddedMatrix()->setEdgeList(&edge_list_upd2);
	matrix.getNewlyAddedMatrix()->setStartEdges(&start_edge_upd2);

	vector<long> start_edge_upd3;
	vector<long> edge_list_upd3;
	start_edge_upd3.push_back(0);
	start_edge_upd3.push_back(0);
	start_edge_upd3.push_back(1);
	start_edge_upd3.push_back(3);
	edge_list_upd3.push_back(5);
	edge_list_upd3.push_back(4);
	edge_list_upd3.push_back(3);

	matrix.getDeletedMatrix()->setEdgeList(&edge_list_upd3);
	matrix.getDeletedMatrix()->setStartEdges(&start_edge_upd3);

	node.setMatrix(&matrix);
	node.setSeed(13);// implies 1358590890 pseudo number, 0.632643 uni rand,
  // 0.341416 for second time
  long edge;

	RandomWalk rw;
	rw.edge_num = 2;
	rw.buf[0] = 3;
	rw.buf[1] = 4;
	rw.buf[2] = 3;
	rw.fp_index = 0;

	// at the end of path we have to generate
	node.setEdgeInPathes(2);
	edge = node.nextEdge(&rw);
	ASSERT_EQ(4, rw.buf[3]);
	ASSERT_EQ(3, rw.edge_num);

	node.setEdgeInPathes(3);
	node.nextEdge(&rw);
	ASSERT_EQ(4, rw.buf[4]);
	ASSERT_EQ(4, rw.edge_num);

	node.setEdgeInPathes(4);
	node.nextEdge(&rw);
	ASSERT_EQ(3, rw.buf[5]);
	ASSERT_EQ(5, rw.edge_num);

  // edge deleted we have to generate
	rw.buf[0] = 4;
	rw.buf[1] = 5;
	node.setEdgeInPathes(0);
	node.nextEdge(&rw);
  ASSERT_EQ(4, rw.buf[1]);
  ASSERT_EQ(1, rw.edge_num);

  // not deleted but we stay
  rw.edge_num = 5;
  rw.buf[0] = 3;
  rw.buf[1] = 5;
  edge = node.nextEdge(&rw);
  ASSERT_EQ(5, rw.buf[1]);
  ASSERT_EQ(5, rw.edge_num);
  ASSERT_EQ(-2, edge);

  // not deleted and we stay
  rw.buf[0] = 4;
  rw.buf[1] = 3;
  node.setEdgeInPathes(1);
  edge = node.nextEdge(&rw);
  ASSERT_EQ(3, rw.buf[2]);
  ASSERT_EQ(5, rw.edge_num);
  ASSERT_EQ(-2, edge);

  // not deleted and we stay
  rw.buf[0] = 3;
  rw.buf[1] = 5;
  node.setEdgeInPathes(0);
  edge = node.nextEdge(&rw);
  ASSERT_EQ(5, rw.buf[1]);
  ASSERT_EQ(5, rw.edge_num);
  ASSERT_EQ(-2, edge);

  // not deleted and we stay
  // should have a fake random generator
	rw.buf[0] = 3;
	rw.buf[1] = 5;
	node.setEdgeInPathes(0);
	edge = node.nextEdge(&rw);
	ASSERT_EQ(5, rw.buf[1]);
	ASSERT_EQ(1, rw.edge_num);
	ASSERT_FALSE(-2 == edge);
}


}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


