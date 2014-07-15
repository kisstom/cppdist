/*
 * fpTreeLeaves.h
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#ifndef FPTREELEAVES_H_
#define FPTREELEAVES_H_

#include <set>
#include <tr1/unordered_map>
#include <stdio.h>
#include <sstream>

class FpTreeLeaves {
public:
  FpTreeLeaves();
  void run(FILE* f);
  long getRoot(char * s);
  void addInnerNodes(char* s, long root);
  ~FpTreeLeaves();

  std::tr1::unordered_map<long, std::set<long> > nodes;
  std::stringstream* tmpStream;
};


#endif /* FPTREELEAVES_H_ */
