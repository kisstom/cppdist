/*
 * test_random_bit_vector.cpp
 *
 *  Created on: 2014.08.13.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <iostream>

#include "../../../main/algos/bitprop/random_bitvector_generator.h"
#include "../../../main/common/random/fake_random_generator.h"

using std::cout;

namespace {

class RandomBitVectorGeneratorTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  RandomBitVectorGeneratorTest() {
  }

  virtual ~RandomBitVectorGeneratorTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

};


TEST_F(RandomBitVectorGeneratorTest, test) {
  vector<double> fakeUniRand;
  fakeUniRand.push_back(0.01);
  fakeUniRand.push_back(0.11);
  fakeUniRand.push_back(0.11);
  fakeUniRand.push_back(0.01);
  fakeUniRand.push_back(0.11);
  fakeUniRand.push_back(0.11);
  fakeUniRand.push_back(0.01);
  fakeUniRand.push_back(0.01);

  fakeUniRand.push_back(0.01);
  fakeUniRand.push_back(0.11);
  fakeUniRand.push_back(0.01);
  fakeUniRand.push_back(0.01);
  fakeUniRand.push_back(0.11);
  fakeUniRand.push_back(0.11);
  fakeUniRand.push_back(0.01);
  fakeUniRand.push_back(0.01);

  FakeRandomGenerator* generator = new FakeRandomGenerator(fakeUniRand);
  RandomBitvectorGenerator rvbgen(0.1, generator);

  long nodes = 2;
  int bytes = 1;

  unsigned char* rvb = new unsigned char[nodes * bytes];
  rvbgen.gen(2, rvb);

  ASSERT_EQ(147, (int)*(rvb + 0));
  ASSERT_EQ(179, (int)*(rvb + 1));
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}







