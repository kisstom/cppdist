/*
 * simrank.cpp
 *
 *  Created on: 2014.01.29.
 *      Author: mrita
 */

#include "simrank.h"


Simrank::Simrank( EdgelistContainer* matrix_,  PseudoRandom* random_) {
  logger = &log4cpp::Category::getInstance(std::string("SimrankOnOneComputer"));
  matrix = matrix_;
  random = random_;
}

void Simrank::getFingerprint( string outFileName, short fpnum, short pathlength ){
  logger->info("Start making fingerprint.");
 
  ofstream outfile (outFileName.c_str());
  if (outfile.is_open()){
    long numnodes =  matrix->getNumberOfNodes();
    for (long n = 0; n < numnodes; ++n) {
      for (short i = 0; i < fpnum; ++i){
	long u = n;
	stringstream path (stringstream::in | stringstream::out );
	//	path << n << " " << i; 
	path << i << " " << n; 
	for (short j = 0; j < pathlength; ++j){
	  u = getRandomNeighbor(u, i, j);
	  if (u==-1) break;
	 path << " " << u ;
	}
	outfile << path.str() << '\n';
      }
      if (n % 100000 == 0 &&  n != 0){
	logger->info("Fingerprint of %ldth node is ready.", n);
      }
    }
    outfile.close();
    logger->info("Fingerprints made.");
   }else{
    logger->error("Unable to open file: %s.", outFileName.c_str());
   }
}

long Simrank::getRandomNeighbor(long node, short i, short j){
  long numneighbor = matrix->neighborhoodSize(node);
  if (numneighbor == 0) return -1;
  int hashValue = random->get(i, j, node);
  int index = hashValue % numneighbor;  
  if (index < 0) index+=numneighbor;
  return matrix->getEdgeAtPos(node, index);
}
