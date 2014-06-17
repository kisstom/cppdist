/*
 * psimrank_random_generator.cpp
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#include "psimrank_random_generator.h"


PSimrankRandomGenerator::PSimrankRandomGenerator(int seed) {
  seed_ = seed;
  gmp_randinit_default (r_state_);
  gmp_randseed_ui(r_state_, seed_);
}

long PSimrankRandomGenerator::generateRandomCoeff() {
  mpz_t random;
  mpz_init(random);
  mpz_urandomm(random, r_state_, prime_);

  long r = mpz_get_ui(random);
  mpz_clear(random);

  return r;
}


void PSimrankRandomGenerator::setPrime(mpz_t* prime, long n) {
  mpz_t mpz_num_nodes;
  mpz_init(mpz_num_nodes);
  mpz_set_ui(mpz_num_nodes, n);

  mpz_init(prime_);
  mpz_init(*prime);

  mpz_nextprime(prime_, mpz_num_nodes);
  mpz_set_ui(*prime, mpz_get_ui(prime_));

  mpz_clear(mpz_num_nodes);
}
