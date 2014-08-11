/*
 * random_bitvector_generator.cpp
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#include "random_bitvector_generator.h"
#include <cstdlib>

void RandomBitvectorGenerator::gen(int numBytes, unsigned char* dest) {
  int v;
  for (int b = 0; b < numBytes; ++b) {
    v = (rand() < posProb);
    for (int i = 0; i < 7; ++i) {
      v <<= 1;
      v |= (rand() < posProb);
    }
    *(dest + b) = v;
  }
}


