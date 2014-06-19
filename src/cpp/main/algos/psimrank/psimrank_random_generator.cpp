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
  gmp_randseed_ui(r_state_, (long) seed_);
}

long PSimrankRandomGenerator::generateRandomAdd(long n) {
  mpz_t random, mpz_num_nodes;
  mpz_init(random);
  mpz_init(mpz_num_nodes);
  mpz_set_ui(mpz_num_nodes, n);

  mpz_urandomm(random, r_state_, mpz_num_nodes);

  long r = mpz_get_ui(random);
  mpz_clear(random);
  mpz_clear(mpz_num_nodes);

  return r;
}

long PSimrankRandomGenerator::generateRandomPrime(long n) {
  mpz_t mpz_num_nodes, prime;
  mpz_init(mpz_num_nodes);
  mpz_set_ui(mpz_num_nodes, n);

  mpz_init(prime);
  mpz_nextprime(prime, mpz_num_nodes);
  long retval = mpz_get_ui(prime);
  mpz_clear(mpz_num_nodes);

  return retval;
}
