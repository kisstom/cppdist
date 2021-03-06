/*
 * counter_inverse_node.h
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#ifndef COUNTER_INVERSE_NODE_H_
#define COUNTER_INVERSE_NODE_H_

#include "../algo_components/old_partition_node.h"
#include "../../common/graph/edgelist_container.h"
#include "../../common/components/mutex.h"
#include <stdio.h>
#include <gtest/gtest_prod.h>
#include "../algo_components/factories/graph_partition_config_handler.h"

struct InverseTriple {
  InverseTriple(long c, long t, short _fromPartition):
    count(c), to(t), fromPartition(_fromPartition) {}

  static bool compare(const InverseTriple& lh, const InverseTriple& rh) {
    if (lh.to == rh.to) return lh.fromPartition < rh.fromPartition;
    return lh.to < rh.to;
  }

  long count;
  long to;
  short fromPartition;
};


class CounterInverseNode : public OldPartitionNode {
public:
  CounterInverseNode();
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void final();
  void setOutputFile(string outputFile);
  void setPartitionBoundFile(string file);
  void setEdgeListContainer(EdgelistContainer*);
  void setPartitionConfigHandler(GraphPartitionConfigHandler*);
  void initFromMaster(string) {}
  void update(short partIndex, long from, long to);
  void determineBounds();
  void setCounters(int numPart);
private:
  void serializeEdge(int partIndex, long from, long to);
  EdgelistContainer* matrix;
  vector<InverseTriple>* inversePartsEdges;
  GraphPartitionConfigHandler* partConfHandler;
  vector<long>* counter;
  vector<long>* newComer;
  vector<long>* partitionBound;
  vector<long>* bounds;

  Mutex mutex;
  string outfile;
  string partitionBoundFile;
  log4cpp::Category* logger_;

  // TEST CASES
  FRIEND_TEST(CounterInverseNodeTest, test);
};


#endif /* COUNTER_INVERSE_NODE_H_ */
