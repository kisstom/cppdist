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

class InfectedTreeComputer {
public:
  InfectedTreeComputer(std::tr1::unordered_map<long, std::set<long> >*,
      std::tr1::unordered_map<int, std::set<long> >*);

  void computeInfectedTrees();
  void flush(FILE*);
  std::tr1::unordered_map<long, int> infectedTreeCount;
private:
  std::tr1::unordered_map<long, std::set<long> >* fpTreeNodes;
  std::tr1::unordered_map<int, std::set<long> >* infectedNodes;
  Util util;
};


#endif /* INFECTEDTREECOMPUTER_H_ */
