/*
 * infectedNodeComputer.h
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#ifndef INFECTEDNODECOMPUTER_H_
#define INFECTEDNODECOMPUTER_H_

#include <stdio.h>
#include <vector>
#include <tr1/unordered_map>
#include <set>

class InfectedNodeComputer {
public:
  InfectedNodeComputer(std::vector<long>);
  void buildFromFile(FILE*);
  int findInfectedIndex(long);
  void addInfectedNodes(const std::vector<long>& edges);
  void incrementNumLine();
  std::tr1::unordered_map<int, std::set<long> > infectedNodes;
private:
  std::vector<long> crawlMaxes;
  long maxNodeId;
  long lineNumber;
};


#endif /* INFECTEDNODECOMPUTER_H_ */
