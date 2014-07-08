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
#include <sstream>
#include <tr1/unordered_map>
#include "param_miss_exception.h"
#include <stdarg.h>

using std::vector;
using std::stringstream;
using std::string;
using std::tr1::unordered_map;

class Util {
public:
	static size_t nextLong(char* line, size_t from, long& element);
	static size_t nextShort(char* line, size_t from, short& element);
	static void readEdges(char* line, vector<long>* edges);
	static bool search(long elem, vector<long>::iterator, int size);
	static bool hasNext(char*, size_t from);
	static void split(char* line, vector<long>& edges);
	vector<string> split(string str, char sep);
	vector<long> convertToLong(vector<string>);
	void checkParam(vector<string> , unordered_map<string, string>*);
	void checkParam(unordered_map<string, string>*, int argc, ...);
};


#endif /* UTIL_H_ */
