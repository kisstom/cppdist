#include <gtest/gtest.h>

#include "../../../main/algos/simrank_store_first/simrank_store_first_node.h"

namespace {

class SimrankStoreFirstTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SimrankStoreFirstTest() {
  }

  virtual ~SimrankStoreFirstTest() {
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

	TEST(SimrankStoreFirstTest, testAfterIteration) {
		SimrankStoreFirstNode node;
		vector<vector<long*> > finished;
		finished.resize(3);
		node.initFinishedPathes(finished);

		vector<list<long*> > fp;
		list<long*> pathes;
		pathes.push_back(new long[2]);
		pathes.front()[0] = 0;
		pathes.push_back(new long[2]);
		pathes.front()[0] = 1;
		pathes.push_back(new long[2]);
		pathes.front()[0] = 2;

		/*pathes[0][0] = 0;
		pathes[1][0] = 1;
		pathes[2][0] = 2;*/

		fp.push_back(pathes);
		node.setFingerprints(fp);

		unordered_map<long, long> nextNodes;
		nextNodes[0] = 10;
		nextNodes[1] = -1;
		nextNodes[2] = 11;

		node.setNextNodes(nextNodes);

		node.afterIteration();
		ASSERT_EQ(2, (int) node.getPathes()->at(0).size());
		ASSERT_EQ(1, (int) node.getFinishedPathes()->at(0).size());


		long* resultPathes = (*node.getPathes())[0].front();
		ASSERT_EQ(1, resultPathes[0]);
		ASSERT_EQ(10, resultPathes[1]);

		(*node.getPathes())[0].pop_front();
		resultPathes = (*node.getPathes())[0].front();
		ASSERT_EQ(11, resultPathes[1]);

	}
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
