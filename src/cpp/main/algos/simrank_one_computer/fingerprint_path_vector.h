/*
 * fingerprint_path_file.h
 *
 *  Created on: 2014.02.03.
 *      Author: mrita
 */

#ifndef FINGERPRINT_PATH_VECTOR_H_
#define  FINGERPRINT_PATH_VECTOR_H_

#include <string>
#include <log4cpp/Category.hh>
#include <sstream>

# include "fingerprint_path.h"

using namespace std;

class FingerprintPathVector: public FingerprintPath {
 private:
    int seed;
    vector<string>* svector;
    log4cpp::Category* logger;
    EdgelistContainer* matrix;
    PseudoRandom* random;
    long getRandomNeighbor(long node, short i, short j);
 public:
    FingerprintPathVector(int seed,  EdgelistContainer* matrix, 
			PseudoRandom* random);
    ~FingerprintPathVector();
    void make(long node, short fp, short pathlength);
    void init();
    void finish();
    vector<string>* getVector();
};

#endif /* FINGERPRINT_PATH_VECTOR_H_ */
