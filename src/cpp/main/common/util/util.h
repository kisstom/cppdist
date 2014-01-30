/*
 * util.h
 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <string.h>
#include <vector>

using std::vector;

class Util {
public:
	static size_t nextLong(char* line, size_t from, long& element);
	static size_t nextShort(char* line, size_t from, short& element);
	static void readEdges(char* line, vector<long>* edges);
	static bool search(long elem, vector<long>::iterator, int size);
	static bool hasNext(char*, size_t from);
};


#endif /* UTIL_H_ */
