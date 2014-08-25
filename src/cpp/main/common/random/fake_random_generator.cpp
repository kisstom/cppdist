/*
 * fake_random_generator.cpp
 *
 *  Created on: 2014.08.15.
 *      Author: kisstom
 */

#include "fake_random_generator.h"

FakeRandomGenerator::FakeRandomGenerator(vector<double> _fakeRandom) {
  fakeRandom = _fakeRandom;
  pos = 0;
}

double FakeRandomGenerator::uniRand() {
  double retval = fakeRandom[pos];

  if (++pos >= (int) fakeRandom.size()) {
    pos = 0;
  }

  return retval;
}
