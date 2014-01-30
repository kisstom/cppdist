/*
 * pseudo_random_generator.cpp
 *
 *  Created on: 2014.01.23.
 *      Author: kisstom
 */

#include "pseudo_random_generator.h"

PseudoRandomGenerator::PseudoRandomGenerator(int seed, short* fpIndex, short* pathIndex) {
	pseudoRandom_ = new HashPseudoRandom(seed);
	fpIndex_ = fpIndex;
	pathIndex_ = pathIndex;
	logger_ = &log4cpp::Category::getInstance(std::string("PseudoRandomGenerator"));
}

double PseudoRandomGenerator::uniRand(long node) {
	return pseudoRandom_->get(*fpIndex_, *pathIndex_, node) / (double) UINT_MAX;
}


