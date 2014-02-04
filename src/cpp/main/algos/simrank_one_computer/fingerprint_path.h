/*
 * fingerprint_path.h
 *
 *  Created on: 2014.02.03.
 *      Author: mrita
 */

#ifndef FINGERPRINT_PATH_H_
#define  FINGERPRINT_PATH_H_

#include "../../common/graph/edgelist_container.h"
#include "../../common/random/pseudo_random.h"

class FingerprintPath {
  protected:
    EdgelistContainer* matrix;
    int seed;
    log4cpp::Category* logger;
    PseudoRandom* random;
  public:
    virtual long getRandomNeighbor(long node, short i, short j) = 0;
    virtual void make(long node ,short fp, short pathlength) = 0;
    virtual void init() = 0;
    virtual void finish() = 0;
};

#endif /* FINGERPRINT_PATH_H_ */
