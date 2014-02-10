/*
 * simrank_one_computer.cpp
 *
 *  Created on: 2014.01.30.
 *      Author: mrita
 */

#include <log4cpp/Category.hh>
#include <iostream>

#include "read_from_file.h"
#include "simrank.h"
#include "random_generator_type.h"
#include "fingerprint_path_type.h"
#include "../../common/random/hash_pseudo_random.h"
#include "../../common/random/build_in_pseudo_random.h"
#include "fingerprint_path_file.h"
#include "fingerprint_path_vector.h"

using namespace std;

PseudoRandom* initRandomGenerator(int seed, GeneratorType gtype){
  PseudoRandom* random;
  log4cpp::Category* logger;
  logger = &log4cpp::Category::getInstance(std::string("initRandomGenerator"));
  if(gtype == HASH_PSEUDO_RANDOM) {
    random =  new HashPseudoRandom(seed);
    logger->info("HASH_PSEUDO_RANDOM generator is set.");
   }else if(gtype == BUILD_IN_PSEUDO_RANDOM){
    random = new BuildInPseudoRandom(seed); 
    logger->info("BUILD_IN_PSEUDO_RANDOM generator is set.");
  }else{
    logger->info("Unknown type of random generator %d", gtype);
     }
  return random;
}

FingerprintPath* initFingerprintPath(FingerprintPathType ftype, int seed, string outFileName, EdgelistContainer* matrix, 
			PseudoRandom* random){
  FingerprintPath* fppath;
  log4cpp::Category* logger;
  logger = &log4cpp::Category::getInstance(std::string("initFingerprintPath"));
  if(ftype == FINGERPRINT_FILE) {
    fppath =  new FingerprintPathFile(seed, outFileName, matrix, random);
    logger->info("FINGERPRINT_PATH_FILE is set.");
   }else if(ftype ==FINGERPRINT_VECTOR){
    fppath = new FingerprintPathVector(seed, matrix, random); 
    logger->info(" FINGERPRINT_PATH_VECTOR is set.");
  }else{
    logger->info("Unknown type of random generator %d", ftype);
     }
  return fppath;
}

int main(int argc, char *argv[]) {
  if(argc != 8){
    cout << "params are: graph input file, fingerprint output file, seed, type of random generator (0:HASH_PSEUDO_RANDOM, 1:BUILD_IN_PSEUDO_RANDOM)," <<
      "type of fingerprint path (0:FINGERPRINT_FILE, 1:FINGERPRINT_VECTOR), how many fingerprints, length of paths" << endl;
  }else{
    ReadFromFile* readUtil = new ReadFromFile();
    EdgelistContainer* matrix = readUtil->read(argv[1]);
    GeneratorType gtype;
    if(atoi(argv[4]) == 0){
      gtype = HASH_PSEUDO_RANDOM;
    }else if(atoi(argv[4]) == 1){
      gtype = BUILD_IN_PSEUDO_RANDOM;
    }
    FingerprintPathType ftype;
    if(atoi(argv[5]) == 0){
      ftype = FINGERPRINT_FILE;
    }else if(atoi(argv[5]) == 1){
      ftype = FINGERPRINT_VECTOR;
    }
    PseudoRandom* random = initRandomGenerator(atoi(argv[3]), gtype);
    FingerprintPath* fppath = initFingerprintPath(ftype, atoi(argv[3]), argv[2],  matrix, random);
    Simrank* simrank = new Simrank(matrix, random, fppath);
    simrank->getFingerprint(atoi(argv[6]), atoi(argv[7]));
  }
  return 0;
}
