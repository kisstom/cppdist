/*
 * random.cpp
 *
 *  Created on: 2014.01.30.
 *      Author: kisstom
 */

#include "random.h"

Random::Random(int seed) {
  seed_ = seed;

	if (seed == -1) {
		timeval tim;
		gettimeofday(&tim, NULL);
		srand(tim.tv_usec);
		return;
	}

  srand(seed_);
}



int Random::get(short fp, short step, long node) {
	return rand();
}
