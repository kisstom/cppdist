#include <gtest/gtest.h>
#include <vector>

using std::vector;

namespace {

class NextEdgeTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	NextEdgeTest() {
  }

  virtual ~NextEdgeTest() {
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
}

TEST(NextEdgeTest, test) {
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
	node.setSeed(13);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
