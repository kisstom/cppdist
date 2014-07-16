/*
 * infectedNodeComputer.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include "infectedNodeComputer.h"
#include <string.h>
#include <algorithm>
#include <iostream>
#include "../common/util/util.h"


InfectedNodeComputer::InfectedNodeComputer(std::vector<long> _crawlMaxes) {
  crawlMaxes = _crawlMaxes;
  maxNodeId = crawlMaxes[0];
  lineNumber = 0;
}

void InfectedNodeComputer::buildFromFile(FILE* input) {
  const int ROW_LEN = 15000000;
  char* line = new char[ROW_LEN];
  std::vector<long> edges;

  while (fgets(line, ROW_LEN, input)) {
    if (lineNumber > maxNodeId) {
      break;
    }

    if (strlen(line) == 1) {
      incrementNumLine();
      continue;
    }

    line[strlen(line)-1] = '\0';

    Util::split(line, edges);
    std::sort (edges.begin(), edges.end());
    addInfectedNodes(edges);

    incrementNumLine();
  }

  delete[] line;
}

void InfectedNodeComputer::incrementNumLine() {
  ++lineNumber;
}

void InfectedNodeComputer::addInfectedNodes(const std::vector<long>& edges) {
  int infectedIndex;

  for (std::vector<long>::const_iterator it = edges.begin(); it != edges.end(); ++it) {
    infectedIndex = findInfectedIndex(*it);
    if (infectedIndex > 0) {
      infectedNodes[infectedIndex].insert(lineNumber);
      break;
    }
  }
}

int InfectedNodeComputer::findInfectedIndex(long node) {
  int infectedIndex = 0;
  for (; infectedIndex < (int) crawlMaxes.size(); ++infectedIndex) {
    if (crawlMaxes[infectedIndex] >= node) break;

    if (infectedIndex >= (int) crawlMaxes.size() - 1) {
      infectedIndex = -1;
      break;
    }
  }

  return infectedIndex;
}


