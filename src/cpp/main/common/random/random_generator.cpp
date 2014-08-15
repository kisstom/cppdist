/*
 * random_generator.cpp
 *
 *  Created on: 2013.07.31.
 *      Author: kisstom
 */

#include "random_generator.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

RandomGenerator::RandomGenerator(int seed) {
	if (seed == -1) {
		timeval tim;
		gettimeofday(&tim, NULL);
		srand(tim.tv_usec);
		return;
	}

	srand(seed);
}

double RandomGenerator::uniRand() {
	return rand() / (double) RAND_MAX;
}



