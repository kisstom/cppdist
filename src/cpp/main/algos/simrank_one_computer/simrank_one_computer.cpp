/*
 * simrank_one_computer.cpp
 *
 *  Created on: 2014.01.30.
 *      Author: mrita
 */

#include <log4cpp/Category.hh>

#include "read_from_file.h"
#include "simrank.h"
#include "random_generator_type.h"
#include "../../common/random/hash_pseudo_random.h"
#include "../../common/random/build_in_pseudo_random.h"


PseudoRandom* initRandomGenerator(int seed, GeneratorType type){
  PseudoRandom* random;
  log4cpp::Category* logger;
  logger = &log4cpp::Category::getInstance(std::string("initRandomGenerator"));
  if(type == HASH_PSEUDO_RANDOM) {
    random =  new HashPseudoRandom(seed);
    logger->info("HASH_PSEUDO_RANDOM generator is set.");
   }else if(type == BUILD_IN_PSEUDO_RANDOM){
    random = new BuildInPseudoRandom(seed); 
    logger->info("BUILD_IN_PSEUDO_RANDOM generator is set.");
  }else{
    logger->info("Unknown type of random generator %d", type);
     }
  return random;
}

int main(int argc, char *argv[]) {
  if(argc != 7){
    cout << "params are: graph input file, fingerprint output file, seed, type of random generator (0:HASH_PSEUDO_RANDOM, 1:BUILD_IN_PSEUDO_RANDOM), how many fingerprints, length of paths" << endl;
  }else{
    ReadFromFile* readUtil = new ReadFromFile();
    EdgelistContainer* matrix = readUtil->read(argv[1]);
    GeneratorType type;
    if(atoi(argv[4]) == 0){
      type = HASH_PSEUDO_RANDOM;
    }else if(atoi(argv[4]) == 1){
      type = BUILD_IN_PSEUDO_RANDOM;
    }
    PseudoRandom* random = initRandomGenerator(atoi(argv[3]), type);
    Simrank* simrank = new Simrank(matrix, random);
    simrank->getFingerprint(argv[2], atoi(argv[5]), atoi(argv[6]));
  }
  return 0;
}
