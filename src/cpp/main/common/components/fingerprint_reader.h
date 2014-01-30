/*
 * fingerprint_reader.h
 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#ifndef FINGERPRINT_READER_H_
#define FINGERPRINT_READER_H_

#include "../old/random_walk.h"
#include "../util/util.h"
#include <list>
#include "log4cpp/Category.hh"
using std::list;

class FingerprintReader {
public:
	FingerprintReader();
	void setRWContainer(list<RandomWalk>*);
	void readWalk(char *, RandomWalk&, long, long);
	void readFile(char* file, long from, long to);
	list<RandomWalk>* getFingerprints();
private:
  list<RandomWalk>* container_;
  log4cpp::Category* logger_;

};


#endif /* FINGERPRINT_READER_H_ */
