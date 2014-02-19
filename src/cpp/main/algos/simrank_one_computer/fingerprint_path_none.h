/*
 * fingerprint_path_none.h
 *
 *  Created on: 2014.02.19.
 *      Author: mrita
 */

#ifndef FINGERPRINT_PATH_NONE_H_
#define  FINGERPRINT_PATH_NONE_H_

#include <string>
#include <log4cpp/Category.hh>
#include <sstream>

# include "fingerprint_path.h"

using namespace std;

class FingerprintPathNone: public FingerprintPath {
 private:
    int seed;
    log4cpp::Category* logger;
    EdgelistContainer* matrix;
    PseudoRandom* random;
    long getRandomNeighbor(long node, short i, short j);
 public:
    FingerprintPathNone(int seed,  EdgelistContainer* matrix, 
			PseudoRandom* random);
    void make(long node, short fp, short pathlength);
    void init();
    void finish();
};

#endif /* FINGERPRINT_PATH_NONE_H_ */
