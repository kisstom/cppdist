/*
 * random.h
 *
 *  Created on: 2014.01.30.
 *      Author: kisstom
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include "pseudo_random.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

class Random: public PseudoRandom {

 public:
    Random(int seed);
    int get(short fp, short step, long node);
 private:
    int seed_;

};


#endif /* RANDOM_H_ */
