/*
 * custom_non_block_node.h
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#ifndef CUSTOM_NON_BLOCK_NODE_H_
#define CUSTOM_NON_BLOCK_NODE_H_

#include <tr1/unordered_map>
#include <map>
#include <vector>
#include "../algo_components/old_partition_node.h"
#include "../algo_components/factories/graph_partition_config_handler.h"

using std::tr1::unordered_map;
using std::vector;
using std::map;

class CustomNonBlockNode: public OldPartitionNode {
public:
  CustomNonBlockNode(long _allnode, long _minnode, double _dump, int _maxIter);
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

  void serializeImportance(short, long, double);
  void updateSenderScore(long fromEdge, double sc);
  void updateReceiverScore(long fromEdge, double sc);
  void readInverseNodeBounds(string);
  void readInverseOutEdges(string);
private:
  vector<short*>* outPartitions;
  unordered_map<long, std::pair<long, long> >* inverseNodeBounds;
  vector<long>* inverseOutEdges;
  vector<int>* numNeighbors;

  vector<double>* pagerankScore_;
  vector<double>* tmpReceiverScore_;
  vector<double>* tmpSenderScore_;
  GraphPartitionConfigHandler* partConfHandler;

  string outfile;
  long allNode_;
  long minNode_;
  double dump_;
  int actIter;
  int maxIter;

  log4cpp::Category* logger_;
};


#endif /* CUSTOM_NON_BLOCK_NODE_H_ */
