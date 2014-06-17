/*
 * psimrank_pseudo_random_generator.cpp
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#include "psimrank_pseudo_random_generator.h"

PSimrankPseudoRandomGenerator::PSimrankPseudoRandomGenerator(
    long prime, vector<long>* randomA, vector<long>* randomB) {
  numRequests_ = 0;
  prime_ = prime;
  randomA_ = randomA;
  randomB_ = randomB;
}

long PSimrankPseudoRandomGenerator::generateRandomCoeff() {
  long retval = -1;
  if (numRequests_ % 2 == 0) {
    if (numRequests_ % 2 < (int) randomA_->size()) {
      retval = (*randomA_)[numRequests_ % 2];
    }
  }

  if (numRequests_ % 2 + 1 < (int) randomB_->size()) {
    retval = (*randomB_)[numRequests_ % 2 + 1];
  }

  ++numRequests_;
  return retval;
}


void PSimrankPseudoRandomGenerator::setPrime(mpz_t* prime, long n) {
  mpz_init(*prime);
  mpz_set_ui(*prime, prime_);
}

