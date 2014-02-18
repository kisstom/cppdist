/*
 * simrank_multithread.cpp
 *
 *  Created on: 2014.02.12.
 *      Author: mrita
 */

#include <log4cpp/Category.hh>
#include <iostream>
#include <ctime>
#include <string>

#include "../simrank_one_computer/read_from_file.h"
#include "../simrank_one_computer/simrank.h"
#include "../simrank_one_computer/random_generator_type.h"
#include "../simrank_one_computer/fingerprint_path_type.h"
#include "../../common/random/hash_pseudo_random.h"
#include "../../common/random/build_in_pseudo_random.h"
#include "../simrank_one_computer/fingerprint_path_file.h"
#include "../simrank_one_computer/fingerprint_path_vector.h"
#include "simrank_thread.h"


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
  if(argc != 9){
    cout << "params are: graph input file, fingerprint output file, seed, type of random generator (0:HASH_PSEUDO_RANDOM, 1:BUILD_IN_PSEUDO_RANDOM)," <<
      "type of fingerprint path (0:FINGERPRINT_FILE, 1:FINGERPRINT_VECTOR), how many fingerprints, length of paths, how many threads" << endl;
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

    time_t time1;
    time_t time2;
    time(&time1);
   
    short pathlength = atoi(argv[7]);
    short numfppath = atoi(argv[6]);
    short numthread = atoi(argv[8]);
    short numsegm = numfppath / numthread;
    short rem = numfppath - (numsegm * numthread);

    
    if (numsegm == 0) numthread = numfppath;

    int seed = atoi(argv[3]);
    string outFileName = argv[2];

    string outname[numthread];
    SimrankThread* thread[numthread];

    PseudoRandom* random[numthread];
    FingerprintPath* fppath[numthread];
    Simrank* simrank[numthread];

    for (short s = 0; s < numthread; s++ ){
      char numstr[100];
      sprintf(numstr, "%d", s);

      outname[s] = outFileName+numstr+".txt";
      random[s] = initRandomGenerator(seed, gtype);
      fppath[s] = initFingerprintPath(ftype, seed, outname[s],  matrix, random[s]);
      simrank[s] = new Simrank(matrix, random[s], fppath[s]);
    }

    for(short s=0; s < rem; s++){
      thread[s]= new SimrankThread(simrank[s],s*(numsegm+1),(s+1)*(numsegm+1),pathlength);
    }
    
    short r= rem;
    for (short s=rem*(numsegm+1); r < numthread; s = s + numsegm){
      thread[r]= new SimrankThread(simrank[r], s, s+numsegm, pathlength);
      r++;
    }
    
    for (short s=0; s<numthread; s++){
      thread[s]->start();
    }

    for (short s=0; s<numthread; s++){
      thread[s]->waitForThread();
    }
     
    time(&time2);
    cout << time1 << "  " << time2 <<endl;
    cout << ctime(&time1) << ctime(&time2) ;
    cout <<"runtime: "<< difftime (time2, time1)<< " s" << endl;
  }
  return 0;
}
