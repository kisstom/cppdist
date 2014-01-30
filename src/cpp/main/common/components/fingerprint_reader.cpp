/*
 * fingerprint_reader.cpp
 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#include "fingerprint_reader.h"
#include <string.h>
#include <stdio.h>
#include <string>

using std::string;

FingerprintReader::FingerprintReader () {
	logger_ = &log4cpp::Category::getInstance(std::string("FingerprintReader"));
}

void FingerprintReader::setRWContainer(list<RandomWalk>* container) {
	container_ = container;
}

void FingerprintReader::readWalk(char* line, RandomWalk& randomWalk, long minnode, long maxnode) {
	size_t from = 0;
	short fp_index;
	long path;

	from = Util::nextShort(line, from, fp_index);
	if (from == string::npos) {
		return;
	}

	randomWalk.fp_index = fp_index;
	short size = 0;
	while (Util::hasNext(line, from)) {
		from += 1;
		from = Util::nextLong(line, from, path);
		randomWalk.buf[size++] = path;
	}

	if (size == 0) randomWalk.edge_num = 0;
	else if (size == 1) printf("ERROR! Dangling edge found.\n");
	else randomWalk.edge_num = size - 1;

	//logger_->info("min %ld max %ld start %ld", minnode, maxnode, randomWalk.buf[0]);
	if (randomWalk.buf[0] >= minnode && randomWalk.buf[0] < maxnode) {
	  container_->push_back(randomWalk);
	}
}

void FingerprintReader::readFile(char* file, long from, long to) {
	FILE* fp_with_pathindex = fopen(file, "r");
  if (NULL == fp_with_pathindex) {
  	printf("Error opening file %s\n.", file);
  	return;
  }

	char line[10000];
	RandomWalk rw;
	//std::fill_n(rw.buf, rw.RWMAX, -1);
	while (fgets(line, 10000, fp_with_pathindex) != NULL) {
		//printf("%s\n", line);
		readWalk(line, rw, from, to);
	}

	fclose(fp_with_pathindex);
}

list<RandomWalk>*FingerprintReader::getFingerprints() {
	return container_;
}


