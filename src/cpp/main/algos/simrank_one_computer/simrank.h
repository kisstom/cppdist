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
#include "../../common/random/pseudo_random.h"
#include "fingerprint_path.h"

using namespace std;

class Simrank {
  private:
    EdgelistContainer* matrix;
    log4cpp::Category* logger;
    PseudoRandom* random;
    FingerprintPath* fppath;
  public:
    Simrank(EdgelistContainer* matrix, PseudoRandom* random, FingerprintPath* fppath);
    void getFingerprint(short fpnum, short pathlength); 
};

#endif /* SIMRANK_H_ */

