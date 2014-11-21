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
#include <set>

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
	void splitByToken(char* line, vector<long>& edges);
	vector<string> split(string str, char sep);
	vector<long> convertToLong(vector<string>);
	void checkParam(vector<string> , unordered_map<string, string>*);
	void checkParam(unordered_map<string, string>*, int argc, ...);

	void printHashSet(unordered_map<long, std::set<long> >);
	void printHashSet(unordered_map<int, std::set<long> >);

	template<typename T>
	bool hasSection(std::set<T>, std::set<T>);

	int stringToInt(const string&);
	string intToString(const int val);

	int digi(int index);
	char* createBufferAtSize(int size, int index);
	void zmqSocketBlock(int);
	void setIpByHost(const char* hostName, char* ip);
};

template<typename T>
bool Util::hasSection(std::set<T> x, std::set<T> y) {
  typename std::set<T>::const_iterator i = x.begin();
  typename std::set<T>::const_iterator j = y.begin();

  while (i != x.end() && j != y.end())
  {
    if (*i == *j)
      return true;
    else if (*i < *j)
      ++i;
    else
      ++j;
  }
  return false;
}

#endif /* UTIL_H_ */
