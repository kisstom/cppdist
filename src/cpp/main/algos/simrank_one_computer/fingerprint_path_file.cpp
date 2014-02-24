/*
 * fingerprint_path_file.cpp
 *
 *  Created on: 2014.02.03.
 *      Author: mrita
 */

# include "fingerprint_path_file.h"

FingerprintPathFile::FingerprintPathFile(int seed_, string outFileName_,  
					 EdgelistContainer* matrix_, PseudoRandom* random_) {
  logger = &log4cpp::Category::getInstance(std::string("FingerprintPathFile"));
  seed = seed_;
  outFileName = outFileName_;
  matrix = matrix_;
  random = random_;
}

void FingerprintPathFile::init(){
  outfile.open(outFileName.c_str());
  if (! outfile.is_open()){
      logger->error("Unable to open file: %s.", outFileName.c_str());
   }
}

void FingerprintPathFile::make(long node, short i, short pathlength){
  long u = node;
  stringstream path (stringstream::in | stringstream::out );
  //	path << n << " " << i; 
  path << i << " " << u; 
  for (short j = 0; j < pathlength; ++j){
    u = getRandomNeighbor(u, i, j);
    if (u==-1) break;
    path << " " << u ;
  }
  outfile << path.str() << '\n';
}

void FingerprintPathFile::finish(){
  outfile.close();
}

long FingerprintPathFile::getRandomNeighbor(long node, short i, short j){
  long numneighbor = matrix->neighborhoodSize(node);
  if (numneighbor == 0) return -1;
 if (numneighbor == -1) return -1;
  int hashValue = random->get(i, j, node);
  int index = hashValue % numneighbor;  
  if (index < 0) index+=numneighbor;
  return matrix->getEdgeAtPos(node, index);
}


