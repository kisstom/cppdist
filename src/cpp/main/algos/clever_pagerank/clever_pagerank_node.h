/*
 * clever_pagerank.h
 *
 *  Created on: 2014.09.09.
 *      Author: kisstom
 */

#ifndef CLEVER_PAGERANK_NODE_H_
#define CLEVER_PAGERANK_NODE_H_

#include <tr1/unordered_map>
#include <map>
#include <vector>
#include "../algo_components/old_partition_node.h"
#include "../../common/components/mutex.h"
#include "../algo_components/factories/graph_partition_config_handler.h"

using std::tr1::unordered_map;
using std::vector;
using std::map;

class CleverPagerankNode: public OldPartitionNode {
public:
  CleverPagerankNode(long _allnode, long _minnode, double _dump, int _maxIter);
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void initFromMaster(string) {}
  void final();

  void setNumberNeighbors(vector<int>*);
  void setOutPartitions(vector<short*>*);
  void setInverseNodeBounds(unordered_map<long, std::pair<long, long> >*);
  void setInverseOutEdges(vector<long>*);
  void setOutputFileName(string);

  void setPartitionConfigHandler(GraphPartitionConfigHandler*);
  void serializeImportance(int, long, double);
  void updateSelfScore(long fromEdge, double sc);
  void readInverseNodeBounds(string);
  void readInverseOutEdges(string);
private:
  vector<short*>* outPartitions;
  unordered_map<long, std::pair<long, long> >* inverseNodeBounds;
  vector<long>* inverseOutEdges;
  vector<int>* numNeighbors;

  vector<double>* pagerankScore_;
  vector<double>* tmpScore_;
  Mutex tmpScoreMutex_;
  GraphPartitionConfigHandler* partConfHandler;

  string outfile;
  long allNode_;
  long minNode_;
  double dump_;
  int actIter;
  int maxIter;

  log4cpp::Category* logger_;
};



#endif /* CLEVER_PAGERANK_NODE_H_ */
