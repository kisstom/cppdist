/*
 * psimrank_master.cpp
 *
 *  Created on: 2014.06.12.
 *      Author: kisstom
 */

#include "psimrank_master.h"

PSimrankMaster::PSimrankMaster() {
  logger_ = &log4cpp::Category::getInstance(std::string("PSimrankMaster"));
  logger_->info("PSimrankMaster inited.");
  iterNum_ = 0;
}

void PSimrankMaster::init() {
  setPrime();
}

bool PSimrankMaster::nextIter() {
  logger_->info("nextIter");

  if (iterNum_++ % 2 == 1) {
    return true;
  }

  long aCoef = generateRandomCoeff();
  long bCoef = generateRandomCoeff();

  char msg[1024];
  sprintf(msg, "%ld %ld", aCoef, bCoef);
  logger_->info("Sending msg %s.", msg);
  master_->sendMessageForAllNodes(msg);

  return true;
}

void PSimrankMaster::addInfoForNodes(char* ss) {
  long long_prime = mpz_get_ui(prime_);
  sprintf(ss, " %ld", long_prime);

}

void PSimrankMaster::setPrime() {
  mpz_t mpz_num_nodes;
  mpz_init(mpz_num_nodes);
  mpz_set_ui(mpz_num_nodes, master_->getNumNodes());

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

