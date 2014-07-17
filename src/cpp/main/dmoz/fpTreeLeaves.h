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
#include "log4cpp/Category.hh"

class FpTreeLeaves {
public:
  FpTreeLeaves();
  void run(FILE* f);
  void addInnerNodes(char* s);
  ~FpTreeLeaves();

  std::tr1::unordered_map<long, std::set<long> > nodes;

private:
  log4cpp::Category* logger;
};


#endif /* FPTREELEAVES_H_ */
