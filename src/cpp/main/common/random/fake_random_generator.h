/*
 * fake_random_generator.h
 *
 *  Created on: 2014.08.15.
 *      Author: kisstom
 */

#ifndef FAKE_RANDOM_GENERATOR_H_
#define FAKE_RANDOM_GENERATOR_H_

#include <vector>
#include "irandom_generator.h"

using std::vector;

class FakeRandomGenerator : public IRandomGenerator {
public:
  FakeRandomGenerator(vector<double>);
  double uniRand();
private:
  vector<double> fakeRandom;
  int pos;
};


#endif /* FAKE_RANDOM_GENERATOR_H_ */
