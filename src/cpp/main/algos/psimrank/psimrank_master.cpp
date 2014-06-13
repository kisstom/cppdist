/*
 * psimrank_master.cpp
 *
 *  Created on: 2014.06.12.
 *      Author: kisstom
 */

#include "psimrank_master.h"

bool PSimrankMaster::nextIter() {
  long aCoef = generateRandomCoeff();
  long bCoef = generateRandomCoeff();

  char msg[1024];
  sprintf(msg, "%ld %ld", aCoef, bCoef);
  master->sendMessageForAllNodes(msg);

  return true;
}

void PSimrankMaster::addInfoForNodes(string* ss) {}

void PSimrankMaster::setPrime() {
  mpz_t mpz_num_nodes;
  mpz_init(mpz_num_nodes);
  mpz_set_ui(mpz_num_nodes, master->getNumNodes());

  mpz_init(prime_);
  mpz_nextprime(prime_, mpz_num_nodes);

  mpz_clear(mpz_num_nodes);
}


long PSimrankMaster::generateRandomCoeff() {
  gmp_randstate_t r_state;
  gmp_randinit_default (r_state);
  gmp_randseed_ui(r_state, 0);

  mpz_t random;
  mpz_init(random);
  mpz_urandomm(random, r_state, prime_);
  return mpz_get_ui(random);
}

