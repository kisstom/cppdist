/*
 * test_find_first_and_last.cpp
 *
 *  Created on: 2014.08.14.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <iostream>

#include "../../../main/algos/bitprop/bitprop_node.h"
using std::cout;

//namespace {

class FindFirstAndLastTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FindFirstAndLastTest() {
  }

  virtual ~FindFirstAndLastTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

};


TEST_F(FindFirstAndLastTest, test) {
  BitpropNode node(1, 1, 0.01, 0);
  std::vector<FailedEstimate>* failedEstimatedNodes = new std::vector<FailedEstimate>();
  failedEstimatedNodes->push_back(FailedEstimate(0.5, 2, 1));
  failedEstimatedNodes->push_back(FailedEstimate(0.5, 3, 1));

  node.neighborhoodDistance = 1;
  node.estIndex = 1;

  int first, last;
  node.findFirstLastIndices(failedEstimatedNodes, &first, &last);
  ASSERT_EQ(0, first);
  ASSERT_EQ(2, last);

}

TEST_F(FindFirstAndLastTest, test2) {
  BitpropNode node(1, 1, 0.01, 0);
  std::vector<FailedEstimate>* failedEstimatedNodes = new std::vector<FailedEstimate>();
  failedEstimatedNodes->push_back(FailedEstimate(0.5, 2, 1));
  failedEstimatedNodes->push_back(FailedEstimate(0.5, 3, 2));

  node.neighborhoodDistance = 1;
  node.estIndex = 1;

  int first, last;
  node.findFirstLastIndices(failedEstimatedNodes, &first, &last);
  ASSERT_EQ(0, first);
  ASSERT_EQ(1, last);
}

TEST_F(FindFirstAndLastTest, test3) {
  BitpropNode node(1, 1, 0.01, 0);
  std::vector<FailedEstimate>* failedEstimatedNodes = new std::vector<FailedEstimate>();
  failedEstimatedNodes->push_back(FailedEstimate(0.5, 2, 1));
  failedEstimatedNodes->push_back(FailedEstimate(0.5, 3, 2));

  node.neighborhoodDistance = 2;
  node.estIndex = 1;

  int first, last;
  node.findFirstLastIndices(failedEstimatedNodes, &first, &last);
  ASSERT_EQ(1, first);
  ASSERT_EQ(2, last);
}

TEST_F(FindFirstAndLastTest, test4) {
  BitpropNode node(1, 1, 0.01, 0);
  std::vector<FailedEstimate>* failedEstimatedNodes = new std::vector<FailedEstimate>();
  failedEstimatedNodes->push_back(FailedEstimate(0.5, 2, 1));
  failedEstimatedNodes->push_back(FailedEstimate(0.5, 3, 2));

  node.neighborhoodDistance = 3;
  node.estIndex = 1;

  int first, last;
  node.findFirstLastIndices(failedEstimatedNodes, &first, &last);
  ASSERT_EQ(0, first);
  ASSERT_EQ(0, last);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


