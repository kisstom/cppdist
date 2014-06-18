/*
 * psimrank_pseudo_random_generator.cpp
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#include "psimrank_pseudo_random_generator.h"

PSimrankPseudoRandomGenerator::PSimrankPseudoRandomGenerator(vector<long> randomPrime, vector<long> randomAdd) {
  numRequests_ = 0;
  randomPrime_ = randomPrime;
  randomAdd_ = randomAdd;
}

long PSimrankPseudoRandomGenerator::generateRandomPrime(long) {
  long retval = -1;

  if (numRequests_ / 2 < (int) randomPrime_.size()) {
    retval = randomPrime_[numRequests_ / 2];
  }

  ++numRequests_;
  return retval;
}


long PSimrankPseudoRandomGenerator::generateRandomAdd(long) {
  long retval = -1;
  if ((numRequests_ - 1) / 2 < (int) randomAdd_.size()) {
    retval = randomAdd_[(numRequests_ - 1) / 2];
  }

  ++numRequests_;
  return retval;
}

