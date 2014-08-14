/*
 * test_random_bit_vector.cpp
 *
 *  Created on: 2014.08.13.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <iostream>

#include "../../../main/algos/bitprop/random_bitvector_generator.h"
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
  RandomBitvectorGenerator rvbgen(0.1, 13);
  long nodes = 4;
  int bytes = 1;

  unsigned char* rvb = new unsigned char[nodes * bytes];
  rvbgen.gen(1, rvb + 0);
  rvbgen.gen(1, rvb + 1);
  rvbgen.gen(1, rvb + 2);
  rvbgen.gen(1, rvb + 3);

  cout << (int)*(rvb + 0) << "|" <<  (int)*(rvb + 1) << "|" <<  (int)*(rvb + 2) << "|" <<  (int)*(rvb + 3) << "\n";
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}







