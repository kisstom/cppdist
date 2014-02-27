/*
 * fingerprint_path_none.cpp
 *
 *  Created on: 2014.02.19.
 *      Author: mrita
 */

# include "fingerprint_path_none.h"

FingerprintPathNone::FingerprintPathNone(int seed_,  EdgelistContainer* matrix_, PseudoRandom* random_) {
  logger = &log4cpp::Category::getInstance(std::string("FingerprintPathNone"));
  seed = seed_;
  matrix = matrix_;
  random = random_;
}

void FingerprintPathNone::init(){
}

void FingerprintPathNone::make(long node, short i, short pathlength){
  long u = node;
  stringstream path (stringstream::in | stringstream::out );
  //	path << n << " " << i; 
  path << i << " " << u; 
  for (short j = 0; j < pathlength; ++j){
    u = getRandomNeighbor(u, i, j);
    if (u==-1) break;
    path << " " << u ;
  }
}

void FingerprintPathNone::finish(){
}



long FingerprintPathNone::getRandomNeighbor(long node, short i, short j){
  long numneighbor = matrix->neighborhoodSize(node);
  if (numneighbor == 0) return -1;
  if (numneighbor == -1) return -1;
  int hashValue = random->get(i, j, node);
  int index = hashValue % numneighbor;  
  if (index < 0) index+=numneighbor;
  return matrix->getEdgeAtPos(node, index);
}
