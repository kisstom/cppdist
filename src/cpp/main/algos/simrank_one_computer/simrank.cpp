/*
 * simrank.cpp
 *
 *  Created on: 2014.01.29.
 *      Author: mrita
 */

#include "simrank.h"


Simrank::Simrank( EdgelistContainer* matrix_,  PseudoRandom* random_, FingerprintPath* fppath_) {
  logger = &log4cpp::Category::getInstance(std::string("SimrankOnOneComputer"));
  matrix = matrix_;
  random = random_;
  fppath = fppath_;
}

void Simrank::getFingerprint(short fpnum, short pathlength ){
  logger->info("Start making fingerprint.");
  fppath->init();
  long numnodes =  matrix->getNumberOfNodes();
  for (long n = 0; n < numnodes; ++n) {
    for (short i = 0; i < fpnum; ++i){
      fppath->make(n, i, pathlength);
    }
    if (n % 100000 == 0 &&  n != 0){
      logger->info("Fingerprint of %ldth node is ready.", n);
    }
  }
  fppath->finish();
  logger->info("Fingerprints made.");
}



