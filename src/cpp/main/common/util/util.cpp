/*
 * util.cpp

 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#include "util.h"
#include <string>
#include <stdio.h>
#include <algorithm>

using std::string;

size_t Util::nextLong(char* line, size_t from, long& element) {
	sscanf(line + from, "%ld", &element);
	for (unsigned i = from; i < strlen(line); ++i) {
		if (line[i] == ' ') {
			return i;
		}
	}
	return string::npos;
}

size_t Util::nextShort(char* line, size_t from, short& element) {
	sscanf(line + from, "%hd", &element);

	for (unsigned i = from; i < strlen(line); ++i) {
		if (line[i] == ' ') {
			return i;
		}
	}
	return string::npos;
}

void Util::readEdges(char* line, vector<long>* edges) {
  size_t from = 0;
  long node;

  if (strlen(line) == 0 || line[0] == '\n') return ;
	from = nextLong(line, from, node);
	edges->push_back(node);

  while (hasNext(line, from)) {
  	from += 1;
  	from = nextLong(line, from, node);
  	edges->push_back(node);
  }
}

bool Util::hasNext(char* line, size_t from) {
	if (from == string::npos) return false;
	for (unsigned i = 0; i < strlen(line + from); ++i) {
		if (line[i + from] == ' ') {
			return true;
		}
	}
	return false;
}

bool Util::search(long elem, vector<long>::iterator it, int size) {
	// TODO: need to be changed to binary search
	vector<long>::iterator res = std::search_n(it, it + size, 1, elem);
	return res != (it + size);
}


