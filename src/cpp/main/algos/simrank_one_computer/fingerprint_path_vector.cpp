/*
 * fingerprint_path_vector.cpp
 *
 *  Created on: 2014.02.03.
 *      Author: mrita
 */

# include "fingerprint_path_vector.h"

FingerprintPathVector::FingerprintPathVector(int seed_,  EdgelistContainer* matrix_, PseudoRandom* random_) {
  logger = &log4cpp::Category::getInstance(std::string("FingerprintPathVector"));
  seed = seed_;
  matrix = matrix_;
  random = random_;
  svector = NULL;
}
FingerprintPathVector::~FingerprintPathVector(){
  if (svector) delete svector;
}

void FingerprintPathVector::init(){
  svector = new vector<string>;
}

void FingerprintPathVector::make(long node, short i, short pathlength){
  long u = node;
  stringstream path (stringstream::in | stringstream::out );
  //	path << n << " " << i; 
  path << i << " " << u; 
  for (short j = 0; j < pathlength; ++j){
    u = getRandomNeighbor(u, i, j);
    if (u==-1) break;
    path << " " << u ;
  }
  svector->push_back(path.str());
}

void FingerprintPathVector::finish(){
}

vector<string>* FingerprintPathVector::getVector(){
  return svector;
}

long FingerprintPathVector::getRandomNeighbor(long node, short i, short j){
  long numneighbor = matrix->neighborhoodSize(node);
  if (numneighbor == 0) return -1;
  int hashValue = random->get(i, j, node);
  int index = hashValue % numneighbor;  
  if (index < 0) index+=numneighbor;
  return matrix->getEdgeAtPos(node, index);
}
