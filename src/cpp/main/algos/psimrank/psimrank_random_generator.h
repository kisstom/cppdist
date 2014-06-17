/*
 * psimrank_random_generator.h
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#ifndef PSIMRANK_RANDOM_GENERATOR_H_
#define PSIMRANK_RANDOM_GENERATOR_H_

#include "psimrank_random_generator_iface.h"

class PSimrankRandomGenerator : public PSimrankRandomGeneratorIFace {
public:
  PSimrankRandomGenerator(int seed);
  long generateRandomCoeff();
  void setPrime(mpz_t*, long n);

private:
  int seed_;
  mpz_t prime_;
  gmp_randstate_t r_state_;
};


#endif /* PSIMRANK_RANDOM_GENERATOR_H_ */
