/*
 * build_in_pseudo_random.h
 *
 *  Created on: 2014.01.20.
 *      Author: mrita
 */

#ifndef BUILD_IN_PSEUDO_RANDOM_H_
#define BUILD_IN_PSEUDO_RANDOM_H_

#include <stdlib.h>

#include "pseudo_random.h"

class BuildInPseudoRandom: public PseudoRandom {
  public:
    BuildInPseudoRandom(int seed_);
    int get(int fp, int step, long node);
};

#endif /* BUILD_IN_PSEUDO_RANDOM_H_ */
