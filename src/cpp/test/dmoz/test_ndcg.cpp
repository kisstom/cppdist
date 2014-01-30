#include <gtest/gtest.h>
#include "../../main/dmoz/ndcg.h"
#include <map>

//using std::string;
using std::vector;
using std::map;

namespace {

class NDCGTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	NDCGTest() {
  }

  virtual ~NDCGTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  static void SetUpTestCase() {
		string fakeInput1 = "fake_input1.txt";
		string fakeInput2 = "fake_input2.txt";
		int topk = 5;
		ndcg = new NDCG(fakeInput1, fakeInput2, topk);
  }

  static void TearDownTestCase() {
    delete ndcg;
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  // Objects declared here can be used by all tests in the test case for Foo.
public:
  static NDCG* ndcg;
};


NDCG* NDCGTest::ndcg = NULL;

TEST(NDCGTest, test1) {
	vector<double> rel;
	rel.push_back(2);
	rel.push_back(1);

	double dcg = NDCGTest::ndcg->dcg(&rel);
	ASSERT_NEAR(3.6309, dcg, 0.001);
}

TEST(NDCGTest, testEmpty) {
	vector<double> rel;

	double dcg = NDCGTest::ndcg->dcg(&rel);
	ASSERT_NEAR(0.0, dcg, 0.00001);
}

TEST(NDCGTest, testNDCGForOneUser) {
  map<long, double> scores1;
  map<long, double> scores2;

  scores1[1] = 1;
  scores1[2] = 2;
  scores1[3] = 3;

  scores2[1] = 3;
  scores2[2] = 1;
  scores2[3] = 2;

  double ndcg = NDCGTest::ndcg->ndcgForOneUser(scores1, scores2);
  ASSERT_NEAR(0.75919, ndcg, 0.00001);
}

TEST(NDCGTest, testNDCGForOneUser2) {
  map<long, double> scores1;
  map<long, double> scores2;

  scores1[1] = 1;
  scores1[2] = 2;
  scores1[3] = 3;

  scores2[1] = 1;
  scores2[2] = 3;
  scores2[3] = 2;

  double ndcg = NDCGTest::ndcg->ndcgForOneUser(scores1, scores2);
  ASSERT_NEAR(0.8428, ndcg, 0.0001);
}

TEST(NDCGTest, testRelevanceByFirst) {
	map<long, double> scores1;
	map<long, double> scores2;

	scores1[1] = 1;
	scores1[2] = 2;
	scores1[3] = 3;

	scores2[1] = 1;
	scores2[2] = 3;
	scores2[3] = 2;
  vector<double>* rel = new vector<double>;

  NDCGTest::ndcg->GetRelevanceByFirst(scores1, scores2, rel);
  ASSERT_EQ(3, (int) rel->size());
  ASSERT_EQ(2, (*rel)[0]);
  ASSERT_EQ(3, (*rel)[1]);
  ASSERT_EQ(1, (*rel)[2]);

  delete rel;
}

TEST(NDCGTest, testRelevanceBySecond) {
	map<long, double> scores;

	scores[1] = 1;
	scores[2] = 3;
	scores[3] = 2;
  vector<double>* rel = new vector<double>;

  NDCGTest::ndcg->GetRelevanceBySecond(scores, rel);
  ASSERT_EQ(3, rel->size());
  ASSERT_EQ(3, (*rel)[0]);
  ASSERT_EQ(2, (*rel)[1]);
  ASSERT_EQ(1, (*rel)[2]);

  delete rel;
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
