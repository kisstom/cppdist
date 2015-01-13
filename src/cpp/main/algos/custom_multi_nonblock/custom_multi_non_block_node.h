/*
 * custom_non_block_node.h
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#ifndef CUSTOM_MULTI_NON_BLOCK_NODE_H_
#define CUSTOM_MULTI_NON_BLOCK_NODE_H_

#include <tr1/unordered_map>
#include <map>
#include <vector>
#include "../algo_components/old_partition_node.h"
#include "../../common/components/socket/multicast_helper.h"
#include "../algo_components/factories/graph_partition_config_handler.h"

using std::tr1::unordered_map;
using std::vector;
using std::map;

class CustomMultiNonBlockNode: public OldPartitionNode {
public:
  CustomMultiNonBlockNode(long _allnode, long _minnode, double _dump, int _maxIter);
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void initFromMaster(string);
  void final();

  void setUpdateFlags(vector<bool>*);
  void setOutpartitionHashes(vector<short>*);
  void setNumneighbors(vector<int>*);
  void setInverseNodeBounds(unordered_map<long, std::pair<long, long> >*);
  void setInverseOutEdges(vector<long>*);
  void setOutputFileName(string);
  void setPartitionConfigHandler(GraphPartitionConfigHandler*);

  void serializeImportance(short, long, double);
  void updateSenderScore(long fromEdge, double sc);
  void updateReceiverScore(long fromEdge, double sc);
  void readInverseNodeBounds(string);
  void readInverseOutEdges(string);

  ~CustomMultiNonBlockNode();
private:
  vector<short>* outPartitionHashes;
  unordered_map<long, std::pair<long, long> >* inverseNodeBounds;
  vector<long>* inverseOutEdges;
  vector<bool>* updateFlags;
  vector<int>* numneighbors;

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

  int numUpdates;
  int messageCounter;
  log4cpp::Category* logger_;
};


#endif /* CUSTOM_MULTI_NON_BLOCK_NODE_H_ */
