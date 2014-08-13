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

RandomBitvectorGenerator::RandomBitvectorGenerator(double _posProb, int _seed) {
  posProb = _posProb;
  seed = _seed;
}

void RandomBitvectorGenerator::gen(int numBytes, unsigned char* dest) {
  int v, r;
  double p;
  for (int b = 0; b < numBytes; ++b) {
    r = rand();
    p = r / (double) RAND_MAX;
    //cout << "random " << p << "\n";
    v = (p < posProb);
    for (int i = 0; i < 7; ++i) {
      v <<= 1;
      r = rand();
      p = r / (double) RAND_MAX;
      //cout << "random " << p << "\n";
      v |= (p < posProb);
    }
    *(dest + b) = v;
    //cout << "random byte " << v << "\n";
  }
}


