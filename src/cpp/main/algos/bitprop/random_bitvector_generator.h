/*
 * random_bitvector_generator.h
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#ifndef RANDOM_BITVECTOR_GENERATOR_H_
#define RANDOM_BITVECTOR_GENERATOR_H_

class RandomBitvectorGenerator {
public:
  RandomBitvectorGenerator(double posProb, int seed);
  void gen(int numBytes, unsigned char* dest);
private:
  double posProb;
  int seed;
};

#endif /* RANDOM_BITVECTOR_GENERATOR_H_ */
