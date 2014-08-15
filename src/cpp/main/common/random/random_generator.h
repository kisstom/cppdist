/*
 * random_generator.h
 *
 *  Created on: 2013.07.31.
 *      Author: kisstom
 */

#ifndef RANDOM_GENERATOR_H_
#define RANDOM_GENERATOR_H_

#include "irandom_generator.h"

class RandomGenerator : public IRandomGenerator {
public:
	RandomGenerator(int);
	double uniRand(long);
};


#endif /* RANDOM_GENERATOR_H_ */
