/*
 * sorted_vector_node_matrix_test.cpp
 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */
#include <gtest/gtest.h>
#include <vector>
#include "../../../main/common/graph/sorted_vector_node_matrix.h"
#include "../../../main/common/graph/edgelist_container.h"
#include <string>
#include "../../common/gtest_environment.h"

using std::vector;
using std::string;

namespace {

class SortedVectorNodeMatrixTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SortedVectorNodeMatrixTest() {
  }

  virtual ~SortedVectorNodeMatrixTest() {
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

TEST(SortedVectorNodeMatrixTest, testGetOldPartitions) {
	SortedVectorNodeMatrix matrix;
  matrix.setMinnode(10);
  matrix.setNumnodes(1);

  char line[1024];
  sprintf(line, "7013 %s/slavery_12.txt 0 5 12\n", testResourceDir.c_str());

  string choose = matrix.checkThisPartition(line).fname;
  ASSERT_EQ("", choose);
}

TEST(SortedVectorNodeMatrixTest, testGetOldPartitions1) {
	SortedVectorNodeMatrix matrix;
  matrix.setMinnode(13);
  matrix.setNumnodes(4);

  char line[1024];
  sprintf(line, "7013 %s/slavery_12.txt 8 5 12\n", testResourceDir.c_str());

  string choose = matrix.checkThisPartition(line).fname;
  string expectedName = testResourceDir + "/slavery_12.txt";
  ASSERT_EQ(expectedName, choose);
}

TEST(SortedVectorNodeMatrixTest, testGetOldPartitions2) {
	SortedVectorNodeMatrix matrix;
  matrix.setMinnode(10);
  matrix.setNumnodes(6);

  char line[1024];
  sprintf(line, "7013 %s/slavery_12.txt 8 5 12\n", testResourceDir.c_str());

  string choose = matrix.checkThisPartition(line).fname;
  string expectedName = testResourceDir + "/slavery_12.txt";
  ASSERT_EQ(expectedName, choose);
}

TEST(SortedVectorNodeMatrixTest, testGetOldPartitions3) {
	SortedVectorNodeMatrix matrix;
  matrix.setMinnode(10);
  matrix.setNumnodes(6);

  char line[1024];
  sprintf(line, "7013 %s/slavery_12.txt 7 5 3\n", testResourceDir.c_str());

  string choose = matrix.checkThisPartition(line).fname;
  ASSERT_EQ("", choose);
}

TEST(SortedVectorNodeMatrixTest, testStoreUpdatedList) {
	SortedVectorNodeMatrix matrix;
	matrix.setMinnode(0);
	matrix.initEdgelistContainers();
	matrix.initEdgelists();
  matrix.getNewlyAddedMatrix()->setMinnode(0);
	matrix.getUnchangedMatrix()->setMinnode(0);
	matrix.getDeletedMatrix()->setMinnode(0);

	vector<long> edge_list_updated;
	edge_list_updated.push_back(1);
	edge_list_updated.push_back(2);
	edge_list_updated.push_back(3);
	vector<long> edge_list_prev;
	edge_list_prev.push_back(0);
	edge_list_prev.push_back(1);

	matrix.storeInUpdatedMatrix(edge_list_updated.begin(), edge_list_updated.end(),
			edge_list_prev.begin(), edge_list_prev.end(),  0);
	matrix.finishMatrix();

  ASSERT_EQ(2, matrix.getNewlyAddedMatrix()->neighborhoodSize(0));
  ASSERT_EQ(1, matrix.getDeletedMatrix()->neighborhoodSize(0));
  ASSERT_EQ(1, matrix.getUnchangedMatrix()->neighborhoodSize(0));
}

TEST(SortedVectorNodeMatrixTest, testContainerInit) {
	SortedVectorNodeMatrix matrix;
	ASSERT_FALSE(matrix.shouldDeleteEdgelistContainers());
	matrix.initEdgelistContainers();
	ASSERT_TRUE(matrix.shouldDeleteEdgelistContainers());
}

TEST(SortedVectorNodeMatrixTest, testIsEdgeDeleted) {
	SortedVectorNodeMatrix matrix;
	vector<long> start_edge_unch;
	vector<long> edge_list_unch;
	vector<long> start_edge_upd;
	vector<long> edge_list_upd;
	vector<long> start_edge_del;
	vector<long> edge_list_del;

	start_edge_unch.push_back(0);
	start_edge_unch.push_back(1);
	start_edge_upd.push_back(0);
	start_edge_upd.push_back(1);
	start_edge_del.push_back(0);
	start_edge_del.push_back(1);

	edge_list_unch.push_back(1);
	edge_list_upd.push_back(2);
	edge_list_del.push_back(3);

	matrix.initEdgelistContainers();
	matrix.getDeletedMatrix()->setMinnode(0);
	matrix.getDeletedMatrix()->setEdgeList(&edge_list_del);
	matrix.getDeletedMatrix()->setStartEdges(&start_edge_del);
	matrix.getUnchangedMatrix()->setEdgeList(&edge_list_unch);
	matrix.getUnchangedMatrix()->setStartEdges(&start_edge_unch);
	matrix.getNewlyAddedMatrix()->setEdgeList(&edge_list_upd);
	matrix.getNewlyAddedMatrix()->setStartEdges(&start_edge_upd);

	ASSERT_TRUE(matrix.isEdgeDeleted(0, 3));
}


/*TEST(SortedVectorNodeMatrixTest, testGetSection) {
	SortedVectorNodeMatrix matrix;
	//matrix.setMinnode(0);

	vector<long> edge_list_old;
	edge_list_old.push_back(1);
	edge_list_old.push_back(2);
	edge_list_old.push_back(3);

	vector<long> edge_list_new;
	edge_list_new.push_back(1);
	edge_list_new.push_back(3);
	edge_list_new.push_back(4);

	vector<long> start_edges_old;
	start_edges_old.push_back(0);
	start_edges_old.push_back(2);
	start_edges_old.push_back(2);
	start_edges_old.push_back(3);
	start_edges_old.push_back(3);

	vector<long> start_edges_new;
	start_edges_new.push_back(0);
	start_edges_new.push_back(2);
	start_edges_new.push_back(2);
	start_edges_new.push_back(3);
	start_edges_new.push_back(3);
	start_edges_new.push_back(3);

	matrix.setOldStartedges(&start_edges_old);
	matrix.setOldEdgelist(&edge_list_old);
	matrix.setUpdatedStartedges(&start_edges_new);
	matrix.setUpdatedEdgelist(&edge_list_new);

  ASSERT_EQ(1, matrix.getUpdatedMatrix()->neighborhoodSize(0));

	vector<long> edge_list1;
	edge_list1.push_back(0);
	edge_list1.push_back(1);
	edge_list1.push_back(2);
	edge_list1.push_back(3);
	edge_list1.push_back(5);
	edge_list1.push_back(7);

  vector<long>* edge_list2 = new vector<long>();
  (*edge_list2).push_back(7);
  (*edge_list2).push_back(2);
  (*edge_list2).push_back(3);
  (*edge_list2).push_back(5);
  (*edge_list2).push_back(6);
  (*edge_list2).push_back(1);

  vector<long>* edge_starts2 = new vector<long>();
  (*edge_starts2).push_back(0);
  (*edge_starts2).push_back(1);
  (*edge_starts2).push_back(5);
  (*edge_starts2).push_back(6);
  (*edge_starts2).push_back(6);

	VectorNodeMatrix updatedMatrix;
	updatedMatrix.SetEdgeList(edge_list2);
	updatedMatrix.SetStartEdges(edge_starts2);


}*/

}

int main (int argc, char **argv) {
	std::string testResourceDir(argc == 2 ? argv[1] : "");
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new MyTestEnvironment(testResourceDir));

  return RUN_ALL_TESTS();
}




