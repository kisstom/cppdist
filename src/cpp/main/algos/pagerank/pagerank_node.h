/*
 * pagerank_node.h
 *
 *  Created on: 2014.04.15.
 *      Author: kisstom
 */

#ifndef PAGERANK_NODE_H_
#define PAGERANK_NODE_H_


#include "../algo.h"
#include "../node.h"
#include "../../common/graph/edgelist_container.h"
#include "../../common/components/mutex.h"
#include <stdio.h>

class PagerankNode : public Node {
public:
  PagerankNode(int maxIter, long allNode, double dump);
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void final();
  void updateScore(long outdge, double sc);
  void setOutputFile(string outputFile);
  void setEdgeListContainer(EdgelistContainer*);
  void initFromMaster(string) {}
private:
  void serializeImportance(int partIndex, long outNode, double importance);
  EdgelistContainer* matrix_;
  vector<double>* pagerankScore_;
  vector<double>* tmpScore_;
  Mutex tmpScoreMutex_;
  string outfile;
  long allNode_;
  double dump_;
  int actIter;
  int maxIter;

  log4cpp::Category* logger_;
};


#endif /* PAGERANK_NODE_H_ */
