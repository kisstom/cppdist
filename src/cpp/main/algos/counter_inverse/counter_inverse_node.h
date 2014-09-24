/*
 * counter_inverse_node.h
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#ifndef COUNTER_INVERSE_NODE_H_
#define COUNTER_INVERSE_NODE_H_

#include "../algo_components/algo.h"
#include "../algo_components/node.h"
#include "../../common/graph/edgelist_container.h"
#include "../../common/components/mutex.h"
#include <stdio.h>

struct InverseTriple {
  InverseTriple(long c, long t, short p):
    count(c), to(t), fromPartition(p) {}

  static bool compare(const InverseTriple& lh, const InverseTriple& rh) {
    return lh.to < rh.to;
  }

  long count;
  long to;
  short fromPartition;
};


class CounterInverseNode : public Node {
public:
  CounterInverseNode();
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void final();
  void setOutputFile(string outputFile);
  void setPartitionBoundFile(string file);
  void setEdgeListContainer(EdgelistContainer*);
  void initFromMaster(string) {}
  void update(short partIndex, long to);
  vector<long> determineBounds();
private:
  void serializeEdge(int partIndex, long to);
  EdgelistContainer* matrix;
  vector<InverseTriple>* inversePartsEdges;
  vector<long>* counter;
  vector<long>* partitionBound;

  Mutex mutex;
  string outfile;
  string partitionBoundFile;
  log4cpp::Category* logger_;
};


#endif /* COUNTER_INVERSE_NODE_H_ */
