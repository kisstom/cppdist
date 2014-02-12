/*
 * simrank.cpp
 *
 *  Created on: 2014.01.29.
 *      Author: mrita
 */

#include "simrank.h"


Simrank::Simrank( EdgelistContainer* matrix_,  PseudoRandom* random_, FingerprintPath* fppath_) {
  logger = &log4cpp::Category::getInstance(std::string("Simrank"));
  matrix = matrix_;
  random = random_;
  fppath = fppath_;
}


void Simrank::getFingerprint( short fpmin, short fpmax, short pathlength ){
  logger->info("Start making fingerprint.");
  fppath->init();
  long numnodes =  matrix->getNumberOfNodes();
  for (long n = 0; n < numnodes; ++n) {
    for (short i = fpmin; i < fpmax; ++i){
      fppath->make(n, i, pathlength);
    }
    if (n % 100000 == 0 &&  n != 0){
      logger->info("Fingerprint of %ldth node is ready.", n);
    }
  }
  fppath->finish();
  logger->info("Fingerprints made.");
}

