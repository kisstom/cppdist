/*
 * file_util.h
 *
 *  Created on: 2013.11.07.
 *      Author: kisstom
 */

#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_

#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <list>

#include "log4cpp/Category.hh"
using std::list;
using std::vector;
using std::string;

class FileUtil {
public:
	FileUtil(int);
	bool readLine(FILE*);
	void readFingerprintStart(long from, long to, short numFingerPrints,
			vector<list<long*> >* fprints, string fname, short bufferSize);
	char** getBufferPointer();
	~FileUtil();
private:
	int maxBufferSize_;
	int initialBufferSize_;
	int bufferSize_;
	char* buffer_;
	log4cpp::Category* logger_;
};


#endif /* FILE_UTIL_H_ */
