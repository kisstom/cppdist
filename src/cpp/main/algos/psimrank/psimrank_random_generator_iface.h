/*
 * psimrank_random_generator_iface.h
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#ifndef PSIMRANK_RANDOM_GENERATOR_IFACE_H_
#define PSIMRANK_RANDOM_GENERATOR_IFACE_H_

#include "gmp.h"
#include "gmpxx.h"

class PSimrankRandomGeneratorIFace {
public:
  virtual long generateRandomAdd(long) = 0;
  virtual long generateRandomPrime(long) = 0;
  //virtual void setPrime(mpz_t*, long n) = 0;
};


#endif /* PSIMRANK_RANDOM_GENERATOR_IFACE_H_ */
