#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../../../main/algos/als/als_util.h"
#include "../../../main/common/graph/entry.h"

namespace {

class AlsUtilTest: public ::testing::Test {
protected:
  AlsUtilTest() {
    ratings = NULL;
    featP = NULL;
    featQ = NULL;
    numUser = 3;
    numItem = 3;
    featDim = 2;
  }

  virtual void SetUp() {
    ratings = new AdjacencyList<Entry>();
    ratings->setMinnode(0);
    ratings->initContainers();

    ratings->addEdge(0, Entry(0, 1.5));
    ratings->addEdge(0, Entry(1, 1.0));
    ratings->addEdge(0, Entry(2, 2.5));
    ratings->addEdge(1, Entry(0, 3.0));
    ratings->addEdge(1, Entry(1, 2.5));
    ratings->addEdge(1, Entry(2, 3.5));
    ratings->addEdge(2, Entry(0, 1.0));
    ratings->addEdge(2, Entry(1, 0.5));
    ratings->addEdge(2, Entry(2, 2.0));
    ratings->setFinish();

    featQ = new FeatureMatrix(numItem, featDim);
    featP = new FeatureMatrix(numUser, featDim);

    featQ->updateEntry(0, 0, 0.1);
    featQ->updateEntry(0, 1, 0.9);
    featQ->updateEntry(1, 0, 0.5);
    featQ->updateEntry(1, 1, 1.2);
    featQ->updateEntry(2, 0, 0.3);
    featQ->updateEntry(2, 1, 1.0);

    featP->updateEntry(0, 0, 0.1);
    featP->updateEntry(0, 1, 0.1);
    featP->updateEntry(1, 0, 0.1);
    featP->updateEntry(1, 1, 0.1);
    featP->updateEntry(2, 0, 0.1);
    featP->updateEntry(2, 1, 0.1);
  }

  virtual void TearDown() {
    delete featP;
    delete featQ;
    delete ratings;
  }

  virtual ~AlsUtilTest() {}

  AdjacencyList<Entry>* ratings;
  FeatureMatrix* featP;
  FeatureMatrix* featQ;
  int numUser;
  int numItem;
  int featDim;
};


TEST_F(AlsUtilTest, test) {
  Entry e(1, 1.0);
  ASSERT_EQ(e.id, 1);
  ASSERT_DOUBLE_EQ(e.value, 1.0);
}

TEST_F(AlsUtilTest, testObjective) {
  long id = 0;
  double lambda;
  gsl_vector* obj = gsl_vector_alloc (featDim);

  AdjacencyListIterator<Entry> ratingRow = ratings->createIterator();
  ratingRow.resetRow(id);
  AlsUtil util(featDim);

  util.setObjective(&ratingRow, featQ, obj);
  ASSERT_NEAR(1.4, gsl_vector_get(obj, 0), 0.0001);
  ASSERT_NEAR(5.05, gsl_vector_get(obj, 1), 0.0001);
}

TEST_F(AlsUtilTest, testSetMatrix) {
  long id = 0;
  double lambda = 0.1;
  gsl_matrix* regrMx = gsl_matrix_alloc (featDim, featDim);

  AdjacencyListIterator<Entry> ratingRow = ratings->createIterator();
  ratingRow.resetRow(id);
  AlsUtil util(featDim);

  util.setRegressionMatrix(featQ, &ratingRow, lambda, regrMx);
  ASSERT_NEAR(0.45, gsl_matrix_get(regrMx, 0, 0), 0.0001);
  ASSERT_NEAR(0.99, gsl_matrix_get(regrMx, 0, 1), 0.0001);
  ASSERT_NEAR(0.99, gsl_matrix_get(regrMx, 1, 0), 0.0001);
  ASSERT_NEAR(3.35, gsl_matrix_get(regrMx, 1, 1), 0.0001);
}

TEST_F(AlsUtilTest, testSolveAndUpdate) {
  AlsUtil util(featDim);
  double lambda = 0.2;
  std::cout << "objective " << util.computeObjective(ratings, featP, featQ, lambda, 0) << '\n';

  util.solveOptimisation(featQ, ratings, 0, featP, lambda);
  util.updateFeature(featP, lambda);
  std::cout << "objective " << util.computeObjective(ratings, featP, featQ, lambda, 0) << '\n';

  std::cout << featP->getEntry(0, 0) << " " << featP->getEntry(0, 1) << '\n';
  std::cout << featP->getEntry(1, 0) << " " << featP->getEntry(1, 1) << '\n';
  std::cout << featP->getEntry(2, 0) << " " << featP->getEntry(2, 1) << '\n';
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
