/*
 * pseudo_random_generator.h
 *
 *  Created on: 2014.01.23.
 *      Author: kisstom
 */

#ifndef PSEUDO_RANDOM_GENERATOR_H_
#define PSEUDO_RANDOM_GENERATOR_H_

#include "pseudo_random.h"
#include <limits.h>

#include "irandom_generator.h"
#include "hash_pseudo_random.h"
#include "log4cpp/Category.hh"

class PseudoRandomGenerator : public IRandomGenerator {
public:
	PseudoRandomGenerator(int, short*, short*);
	double uniRand(long node);
private:
	PseudoRandom* pseudoRandom_;
	short* fpIndex_;
	short* pathIndex_;
	log4cpp::Category* logger_;
};


#endif /* PSEUDO_RANDOM_GENERATOR_H_ */
