/*
 * random_bitvector_generator.cpp
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#include <iostream>
#include "random_bitvector_generator.h"
#include <cstdlib>

using std::cout;

RandomBitvectorGenerator::RandomBitvectorGenerator(double _posProb, IRandomGenerator* _randomGenerator) {
  posProb = _posProb;
  randomGenerator = _randomGenerator;
}

void RandomBitvectorGenerator::gen(int numBytes, unsigned char* dest) {
  int v;
  double p;
  for (int b = 0; b < numBytes; ++b) {
    p = randomGenerator->uniRand();
    v = (p < posProb);
    for (int i = 0; i < 7; ++i) {
      v <<= 1;

      p = randomGenerator->uniRand();
      v |= (p < posProb);
    }
    *(dest + b) = v;
  }
}


