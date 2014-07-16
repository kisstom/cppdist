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

void InfectedTreeComputer::printSet(std::set<long> s, string name) {
  std::cout << name << "\n";
  for (std::set<long>::iterator it = s.begin(); it != s.end(); ++it) {
    std::cout << *it << "\n";
  }
}

void InfectedTreeComputer::computeInfectedTrees() {
  bool hasSection;

  for (std::tr1::unordered_map<long, std::set<long> >::iterator rootIt = fpTreeNodes->begin();
      rootIt != fpTreeNodes->end(); ++rootIt) {
    for (int infectedLevelInd = 1; infectedLevelInd <= (int) infectedNodes->size(); ++infectedLevelInd) {
      hasSection = util.hasSection(rootIt->second, (*infectedNodes)[infectedLevelInd]);

      if (hasSection) {
        for (int j = infectedLevelInd; j <= (int) infectedNodes->size(); ++j) {
          infectedTreeCount[j]++;
        }
        break;
      }
    }
  }
}

void InfectedTreeComputer::flush(FILE* output) {
  for (int j = 1; j <= (int) infectedTreeCount.size(); ++j) {
    fprintf(output, "%d %d\n", j, infectedTreeCount[j]);
  }
}
