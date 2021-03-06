/*
 * infectedTreeComputer.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include "infectedTreeComputer.h"

InfectedTreeComputer::InfectedTreeComputer(std::tr1::unordered_map<RootPath, std::set<long>,
    RootPathFunc, RootPathFunc>* _fpTreeNode,
    std::tr1::unordered_map<int, std::set<long> >* _infectedNodes) {
  fpTreeNodes = _fpTreeNode;
  infectedNodes = _infectedNodes;
  logger_ = &log4cpp::Category::getInstance(std::string("InfectedTreeComputer"));
}

void InfectedTreeComputer::computeInfectedTrees() {
  bool hasSection;

  logger_->info("Starting infected tree count.");
  fflush(stdout);
  int numTreesChecked = 0;
  for (std::tr1::unordered_map<RootPath, std::set<long> >::iterator rootIt = fpTreeNodes->begin();
      rootIt != fpTreeNodes->end(); ++rootIt) {

    // Index starts from 1, because we interested in later crawls and finishes with the size of it.
    for (int infectedLevelInd = 1; infectedLevelInd <= (int) infectedNodes->size(); ++infectedLevelInd) {
      hasSection = util.hasSection(rootIt->second, (*infectedNodes)[infectedLevelInd]);

      if (hasSection) {
        for (int j = infectedLevelInd; j <= (int) infectedNodes->size(); ++j) {
          ++infectedTreeCount[j];
          infectedTreeSize[j] += rootIt->second.size();
        }
        break;
      }
    }

    ++numTreesChecked;
    if (numTreesChecked % 10 == 0) {
      logger_->info("%d trees checked.", numTreesChecked);
      fflush(stdout);
    }
  }
}

void InfectedTreeComputer::flush(FILE* output) {
  for (int j = 1; j <= (int) infectedTreeCount.size(); ++j) {
    fprintf(output, "crawl level: %d tree count: %d\n", j, infectedTreeCount[j]);
    fprintf(output, "crawl level: %d tree sizes: %d\n", j, infectedTreeSize[j]);
  }
}
