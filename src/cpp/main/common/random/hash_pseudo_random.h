/*
 * hash_pseudo_random.h
 *
 *  Created on: 2014.01.20.
 *      Author: mrita
 */

#ifndef HASH_PSEUDO_RANDOM_H_
#define HASH_PSEUDO_RANDOM_H_

#include "pseudo_random.h"

class HashPseudoRandom: public PseudoRandom {
 private:
  long a;
  long b;
  long c;
  long d; 
  long zz; 
  long LLL; 
 public:
    HashPseudoRandom(int seed_);
    int get(short fp, short step, long node);

};

#endif /* HASH_PSEUDO_RANDOM_H_ */
