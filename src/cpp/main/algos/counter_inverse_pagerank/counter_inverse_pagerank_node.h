/*
 * counter_inverse_pagerank_node.h
 *
 *  Created on: 2014.09.24.
 *      Author: kisstom
 */

#ifndef COUNTER_INVERSE_PAGERANK_NODE_H_
#define COUNTER_INVERSE_PAGERANK_NODE_H_

#include "../algo_components/algo.h"
#include "../algo_components/node.h"
#include "../../common/graph/edgelist_container.h"
#include "../../common/components/mutex.h"
#include <gtest/gtest_prod.h>
#include <stdio.h>

class CounterInversePagerankNode : public Node {
public:
  CounterInversePagerankNode(long, double, int);
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void final();
  void initFromMaster(string);
  void update(short partIndex, long from, double imp);
  void updateWithIncomingPr();
  void serializeImportance(int bufferIndex, long from, double importance);

  void setOutputFile(string outputFile);
  void setPointerToCounters(EdgelistContainer*);
  void setOutpartitionIndices(EdgelistContainer*);
  void setPartitionBound(vector<long>*);
  void setNumNeighbors(vector<int>*);
  void readNeighbors(string);
  void initCounters();

  void readPartitionBouns(string fname);
private:
  EdgelistContainer* pointerToCounters;
  EdgelistContainer* outPartitionIndices;

  vector<double>* incomingPageranks;
  vector<double>* pagerankScore;

  vector<long>* counter;
  vector<long>* newComer;
  vector<long>* partitionBound;
  vector<int>* numNeighbors;
  string outfile;

  Mutex mutex;

  long allNode;
  double dump;
  int actIter;
  int maxIter;
  log4cpp::Category* logger_;

  FRIEND_TEST(CounterInversePagerankNodeTest, test);
};



#endif /* COUNTER_INVERSE_PAGERANK_NODE_H_ */
