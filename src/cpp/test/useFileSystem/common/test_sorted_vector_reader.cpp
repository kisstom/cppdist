/*
 * test_sorted_vector_reader.cpp
 *
 *  Created on: 2013.08.23.
 *      Author: kisstom
 */

#include "../../../main/common/graph/sorted_vector_node_matrix.h"
#include <gtest/gtest.h>
#include <vector>
#include "../../../main/common/graph/edgelist_container.h"
#include <string>
#include "../../common/gtest_environment.h"

using std::vector;
using std::string;

namespace {

class SortedVectorNodeMatrixReaderTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SortedVectorNodeMatrixReaderTest() {
  }

  virtual ~SortedVectorNodeMatrixReaderTest() {
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

TEST(SortedVectorNodeMatrixReaderTest, testF) {
	SortedVectorNodeMatrix matrix;
	matrix.setMinnode(0);
	matrix.setNumnodes(4);

	PartitionInfo info;
	info.minnode = 0;
	info.fname = testResourceDir + "/slavery_0.txt_update";

	char line[1024];
	sprintf(line, "%s/slavery.cfg", testResourceDir.c_str());
	matrix.storeMatrixes(info, line);

	EdgelistContainer container;
	container.setMinnode(0);
	container.initContainers();
	container.addEdgeWithMinnode(0, 1);
	container.addEdgeWithMinnode(0, 2);
	container.addEdgeWithMinnode(0, 3);
	container.addEdgeWithMinnode(2, 9);
	container.addEdgeWithMinnode(3, 10);
	container.addEdgeWithMinnode(3, 8);
	container.setFinish();

	EdgelistContainer container2;
	container2.setMinnode(0);
	container2.initContainers();
	container2.setFinish();

	EdgelistContainer container3;
	container3.setMinnode(0);
	container3.initContainers();
	container3.setFinish();

	ASSERT_TRUE(container == (*matrix.getUnchangedMatrix()));
	ASSERT_TRUE(container2 == (*matrix.getNewlyAddedMatrix()));
	ASSERT_TRUE(container3 == (*matrix.getDeletedMatrix()));
}

TEST(SortedVectorNodeMatrixReaderTest, testDifferentConfig) {
	SortedVectorNodeMatrix matrix;
	matrix.setMinnode(0);
	matrix.setNumnodes(4);

	PartitionInfo info;
	info.minnode = 0;
	info.fname = testResourceDir + "/slavery_0.txt";

	char line[1024];
	sprintf(line, "%s/slavery.cfg", testResourceDir.c_str());
	matrix.storeMatrixes(info, line);

	EdgelistContainer unchanged;
	unchanged.setMinnode(0);
	unchanged.initContainers();
	unchanged.addEdgeWithMinnode(2, 9);
	unchanged.addEdgeWithMinnode(3, 10);
	unchanged.addEdgeWithMinnode(3, 8);
	unchanged.setFinish();

	EdgelistContainer deleted;
	deleted.setMinnode(0);
	deleted.initContainers();
	deleted.addEdgeWithMinnode(0, 1);
	deleted.addEdgeWithMinnode(0, 2);
	deleted.addEdgeWithMinnode(0, 3);
	deleted.setFinish();

	EdgelistContainer newlyAdded;
	newlyAdded.setMinnode(0);
	newlyAdded.initContainers();
	newlyAdded.addEdgeWithMinnode(0, 9);
	newlyAdded.addEdgeWithMinnode(0, 10);
	newlyAdded.addEdgeWithMinnode(0, 11);
	newlyAdded.addEdgeWithMinnode(2, 1);
	newlyAdded.addEdgeWithMinnode(2, 2);
	newlyAdded.addEdgeWithMinnode(2, 3);
	newlyAdded.setFinish();

	ASSERT_TRUE(unchanged == (*matrix.getUnchangedMatrix()));
	ASSERT_TRUE(newlyAdded == (*matrix.getNewlyAddedMatrix()));
	ASSERT_TRUE(deleted == (*matrix.getDeletedMatrix()));
}

TEST(SortedVectorNodeMatrixReaderTest, testDifferentConfig2) {
	SortedVectorNodeMatrix matrix;
	matrix.setMinnode(12);
	matrix.setNumnodes(4);

	PartitionInfo info;
	info.minnode = 12;
	info.fname = testResourceDir + "/slavery_2.txt";

	char line[1024];
	sprintf(line, "%s/slavery.cfg", testResourceDir.c_str());
	matrix.storeMatrixes(info, line);

	EdgelistContainer unchanged;
	unchanged.setMinnode(12);
	unchanged.initContainers();
	unchanged.setFinish();

	EdgelistContainer deleted;
	deleted.setMinnode(12);
	deleted.initContainers();
	deleted.setFinish();

	EdgelistContainer newlyAdded;
	newlyAdded.setMinnode(12);
	newlyAdded.initContainers();
	newlyAdded.addEdgeWithMinnode(12, 0);
	newlyAdded.addEdgeWithMinnode(12, 1);
	newlyAdded.addEdgeWithMinnode(12, 2);
	newlyAdded.setFinish();

	ASSERT_TRUE(unchanged == (*matrix.getUnchangedMatrix()));
	ASSERT_TRUE(newlyAdded == (*matrix.getNewlyAddedMatrix()));
	ASSERT_TRUE(deleted == (*matrix.getDeletedMatrix()));
}

}

int main (int argc, char* argv[]) {
	/*std::string testResourceDir(argc == 2 ? argv[1] : "");
	testing::InitGoogleTest(&argc, argv);
	testing::AddGlobalTestEnvironment(new MyTestEnvironment(testResourceDir));

	return RUN_ALL_TESTS();*/
}


