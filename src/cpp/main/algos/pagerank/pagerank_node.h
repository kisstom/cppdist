/*
 * pagerank_node.h
 *
 *  Created on: 2014.04.15.
 *      Author: kisstom
 */

#ifndef PAGERANK_NODE_H_
#define PAGERANK_NODE_H_

#include "../algo_components/old_partition_node.h"
#include "../../common/graph/edgelist_container.h"
#include "../../common/components/mutex.h"
#include <stdio.h>
#include "../algo_components/factories/graph_partition_config_handler.h"

class PagerankNode : public OldPartitionNode {
public:
  PagerankNode(int maxIter, long allNode, double dump);
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void final();
  void updateScore(long outdge, double sc);
  void setOutputFile(string outputFile);
  void setEdgeListContainer(EdgelistContainer*);
  void setPartitionConfigHandler(GraphPartitionConfigHandler*);
  vector<double>* getPagerankScore();
  void initFromMaster(string) {}

  ~PagerankNode();
private:
  friend class PagerankTest;
  void serializeImportance(int partIndex, long outNode, double importance);
  EdgelistContainer* matrix_;
  GraphPartitionConfigHandler* partConfHandler;
  vector<double>* pagerankScore_;
  vector<double>* tmpScore_;
  Mutex tmpScoreMutex_;
  string outfile;
  long allNode_;
  double dump_;
  int actIter;
  int maxIter;

  int messageCounter;
  log4cpp::Category* logger_;
};


#endif /* PAGERANK_NODE_H_ */
