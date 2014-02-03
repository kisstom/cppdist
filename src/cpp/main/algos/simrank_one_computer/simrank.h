/*
 * simrank.h
 *
 *  Created on: 2014.01.29.
 *      Author: mrita
 */

#ifndef SIMRANK_H_
#define SIMRANK_H_

#include <log4cpp/Category.hh>
#include <sstream>
#include <fstream>
#include <iostream>

#include "../../common/graph/edgelist_container.h"
#include "../../common/random/hash_pseudo_random.h"
#include "../../common/random/build_in_pseudo_random.h"
#include "random_generator_type.h"

using namespace std;

class Simrank {
  private:
    EdgelistContainer* matrix;
    int seed;
    log4cpp::Category* logger;
    PseudoRandom* random;
    long getRandomNeighbor(long node, short i, short j);
  
  public:
    Simrank(EdgelistContainer* matrix, PseudoRandom* random);
    void getFingerprint(string outFileName, short fpnum, short pathlength); 
};

#endif /* SIMRANK_H_ */
