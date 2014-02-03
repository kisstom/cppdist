/*
 * build_in_pseudo_random.cpp
 *
 *  Created on: 2014.01.20.
 *      Author: mrita
 */

#include <stdlib.h>

#include "build_in_pseudo_random.h"


BuildInPseudoRandom::BuildInPseudoRandom(int seed_) {
  seed=seed_;
  srand(seed);
};
  
int BuildInPseudoRandom::get(short fp, short step, long node){
  return rand();
}
