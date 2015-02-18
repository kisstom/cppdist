/*
 * infectedTreeComputer.h
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#ifndef INFECTEDTREECOMPUTER_H_
#define INFECTEDTREECOMPUTER_H_

#include "../../main/common/util/util.h"
#include <set>
#include <tr1/unordered_map>
#include <stdio.h>
#include <iostream>
#include "log4cpp/Category.hh"
#include "root_path_key.h"

class InfectedTreeComputer {
public:
  InfectedTreeComputer(std::tr1::unordered_map<RootPath, std::set<long>, RootPathFunc, RootPathFunc>*,
      std::tr1::unordered_map<int, std::set<long> >*);

  void computeInfectedTrees();
  void flush(FILE*);

  std::tr1::unordered_map<long, int> infectedTreeCount;
  std::tr1::unordered_map<long, int> infectedTreeSize;
private:
  std::tr1::unordered_map<RootPath, std::set<long>, RootPathFunc, RootPathFunc>* fpTreeNodes;
  std::tr1::unordered_map<int, std::set<long> >* infectedNodes;
  Util util;
  log4cpp::Category* logger_;
};


#endif /* INFECTEDTREECOMPUTER_H_ */
