/*
 * file_util.cpp
 *
 *  Created on: 2013.11.07.
 *      Author: kisstom
 */
#include "file_util.h"

FileUtil::FileUtil(int initialBufferSize) {
	initialBufferSize_ = initialBufferSize;
	bufferSize_ = initialBufferSize;

	buffer_ = new char[bufferSize_];
	maxBufferSize_ = 100000000;
	logger_ = &log4cpp::Category::getInstance(std::string("FileUtil"));
}

FileUtil::~FileUtil() {
	delete[] buffer_;
}

void FileUtil::readFingerprintStart(long from, long to, short numFingerPrints,
		vector<list<long*> >* fprints, string fname, short numElements) {
	logger_->info("Starting fingerprint start read.");
	fprints->resize(numFingerPrints);

	FILE* fpFile = fopen(fname.c_str(), "r");
	if (fpFile == NULL) {
		logger_->error("Error opening fingerprint start file %s", fname.c_str());
		return;
	}

	long start;
	long* path;
	long numread = 0;
  while (EOF != fscanf(fpFile, "%ld", &start)) {
    if (start < to && start >= from) {
    	++numread;
    	for (int fpIndex = 0; fpIndex < numFingerPrints; ++fpIndex) {
    		path = new long[numElements];
    		std::fill_n (path, numElements, -1);
    		path[0] = start;
    		path[1] = start;
    		(*fprints)[fpIndex].push_back(path);
    	}

    	if (numread % 10000 == 0) {
    		logger_->info("%ld fingerprint start read.", numread);
    	}
    }
  }

	fclose(fpFile);
	logger_->info("Fingerprint start read ended.");
}

bool FileUtil::readLine(FILE* file) {
	int len = 0;
	char* retval;
  while (1) {
  	retval = fgets(buffer_ + len, bufferSize_, file);
  	if (NULL == retval) return false;

  	len = strlen(buffer_);
  	if (buffer_[len - 1] == '\n') {
  		break;
  	}
  	bufferSize_ *= 2;
  	if (bufferSize_ > maxBufferSize_) {
  		logger_->info("Reaching max buffer size %d in file util. Exiting.", maxBufferSize_);
  		return false;
  	}

  	char* newarr = new char[2 * bufferSize_ - initialBufferSize_];
  	strcpy(newarr, buffer_);

  	delete[] buffer_;
  	buffer_ = newarr;
  }
  return true;
}

char** FileUtil::getBufferPointer() {
  return &buffer_;
}



