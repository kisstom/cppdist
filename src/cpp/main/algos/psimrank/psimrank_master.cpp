/*
 * psimrank_master.cpp
 *
 *  Created on: 2014.06.12.
 *      Author: kisstom
 */

#include "psimrank_master.h"

bool PSimrankMaster::nextIter() {
  return true;
}
void PSimrankMaster::addInfoForNodes(string* ss) {}

long PSimrankMaster::generateRandomCoeff() {
  gmp_randstate_t r_state;
  gmp_randinit_default (r_state);
  gmp_randseed_ui(r_state, 0);

  mpz_t random;
  mpz_init(random);
  mpz_urandomm(random, r_state, prime_);
  return mpz_get_ui(random);
}

