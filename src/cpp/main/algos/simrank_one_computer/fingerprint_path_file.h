/*
 * fingerprint_path_file.h
 *
 *  Created on: 2014.02.03.
 *      Author: mrita
 */

#ifndef FINGERPRINT_PATH_FILE_H_
#define  FINGERPRINT_PATH_FILE_H_

#include <string>
#include <log4cpp/Category.hh>
#include <sstream>
#include <fstream>

# include "fingerprint_path.h"

using namespace std;

class FingerprintPathFile: public FingerprintPath {
 private:
    int seed;
    ofstream outfile;
    string outFileName;
    log4cpp::Category* logger;
    EdgelistContainer* matrix;
    PseudoRandom* random;
    long getRandomNeighbor(long node, short i, short j);

  public:
    FingerprintPathFile(int seed, string outFileName, EdgelistContainer* matrix, 
			PseudoRandom* random);
    void make(long node, short fp, short pathlength);
    void init();
    void finish();
};

#endif /* FINGERPRINT_PATH_FILE_H_ */
