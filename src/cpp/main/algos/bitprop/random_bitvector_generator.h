/*
 * random_bitvector_generator.h
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#ifndef RANDOM_BITVECTOR_GENERATOR_H_
#define RANDOM_BITVECTOR_GENERATOR_H_

#include "../../common/random/irandom_generator.h"

class RandomBitvectorGenerator {
public:
  RandomBitvectorGenerator(double posProb, IRandomGenerator*);
  void gen(int numBytes, unsigned char* dest);
private:
  IRandomGenerator* randomGenerator;
  double posProb;
  int seed;
};

#endif /* RANDOM_BITVECTOR_GENERATOR_H_ */
