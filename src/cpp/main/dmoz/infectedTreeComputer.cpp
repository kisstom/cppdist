/*
 * infectedTreeComputer.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include "infectedTreeComputer.h"

InfectedTreeComputer::InfectedTreeComputer(std::tr1::unordered_map<long, std::set<long> >* _fpTreeNode,
      std::tr1::unordered_map<int, std::set<long> >* _infectedNodes) {
  fpTreeNodes = _fpTreeNode;
  infectedNodes = _infectedNodes;
}

void InfectedTreeComputer::computeInfectedTrees() {
  bool hasSection;

  for (std::tr1::unordered_map<long, std::set<long> >::iterator rootIt = fpTreeNodes->begin();
      rootIt != fpTreeNodes->end(); ++rootIt) {
    for (int infectedLevelInd = 1; infectedLevelInd <= infectedNodes->size(); ++infectedLevelInd) {
      hasSection = util.hasSection(rootIt->second, (*infectedNodes)[infectedLevelInd]);

      if (hasSection) {
        for (int j = infectedLevelInd; j <= infectedNodes->size(); ++j) {
          infectedTreeCount[j]++;
        }
      }
    }
  }
}
