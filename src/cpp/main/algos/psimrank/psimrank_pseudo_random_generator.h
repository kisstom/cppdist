/*
 * psimrank_pseudo_random_generator.h
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#ifndef PSIMRANK_PSEUDO_RANDOM_GENERATOR_H_
#define PSIMRANK_PSEUDO_RANDOM_GENERATOR_H_

#include <vector>
#include "psimrank_random_generator_iface.h"

using std::vector;

class PSimrankPseudoRandomGenerator : public PSimrankRandomGeneratorIFace {
public:
  PSimrankPseudoRandomGenerator(vector<long>, vector<long>);
  long generateRandomAdd(long);
  long generateRandomPrime(long);
  //void setPrime(mpz_t*, long n);
private:
  vector<long> randomAdd_;
  vector<long> randomPrime_;
  //long prime_;
  int numRequests_;
};


#endif /* PSIMRANK_PSEUDO_RANDOM_GENERATOR_H_ */
