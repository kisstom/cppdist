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

void Util::split(char* line, vector<long>& edges) {
  edges.clear();
  stringstream ss(stringstream::in | stringstream::out);
  ss << line;
  long edge;

  while (ss.good()) {
    ss >> edge;
    edges.push_back(edge);
  }
}

vector<string> Util::split(string str, char sep) {
  stringstream ss(str);
  std::string token;
  vector<string> retval;
  while(std::getline(ss, token, sep)) {
    retval.push_back(token);
  }

  return retval;
}

vector<long> Util::convertToLong(vector<string> strs) {
  vector<long> retval;
  long tmp;

  for (int i = 0; i < strs.size(); ++i) {
    sscanf(strs[i].c_str(), "%ld", &tmp);
    retval.push_back(tmp);
  }

  return retval;
}

void Util::checkParam(vector<string> shouldContain, unordered_map<string, string>* params) {
  for (int i = 0; i < shouldContain.size(); ++i) {
    if (params->find(shouldContain[i]) == params->end()) {
      throw ParamMissException(shouldContain[i] + " is missing.");
    }
  }
}

void Util::checkParam(unordered_map<string, string>* params, int argc, ...) {
  va_list ap;
  va_start(ap, argc);

  for(int i = 0; i < argc; i++) {
    char* a = va_arg(ap, char*);
    string str_par(a);

    if (params->find(str_par) == params->end()) {
      throw ParamMissException(str_par + " is missing.");
    }
  }
  va_end(ap);
}

void Util::printHashSet(unordered_map<long, std::set<long> > hashSet) {
  for (unordered_map<long, std::set<long> >::iterator it = hashSet.begin(); it != hashSet.end(); ++it) {
    printf("%ld ", it->first);
    for (std::set<long>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
      printf (" %ld", *jt);
    }
    printf("\n");
  }
}

void Util::printHashSet(unordered_map<int, std::set<long> > hashSet) {
  for (unordered_map<int, std::set<long> >::iterator it = hashSet.begin(); it != hashSet.end(); ++it) {
    printf("%d ", it->first);
    for (std::set<long>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
      printf (" %ld", *jt);
    }
    printf("\n");
  }
}

void Util::splitByToken(char* str, vector<long>& edges) {
  char * pch = str;
  long num = 0;

  while (pch != NULL) {
    sscanf(pch, "&ld", &num);
    edges.push_back(num);
    pch = strtok (NULL, " ");
  }
}



