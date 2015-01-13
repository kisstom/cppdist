#ifndef PAGERANK_NON_BLOCK_NODE_H_
#define PAGERANK_NON_BLOCK_NODE_H_

#include "../../common/graph/edgelist_container.h"
#include <tr1/unordered_map>
#include <map>
#include <vector>
#include "../algo_components/node.h"
#include "../algo_components/factories/graph_partition_config_handler.h"

using std::tr1::unordered_map;
using std::vector;
using std::map;

class PagerankNonBlockNode: public Node {
public:
  PagerankNonBlockNode(long _allnode, long _minnode, double _dump, int _maxIter);
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void initFromMaster(string) {}
  void final();

  void setEdgeListContainer(EdgelistContainer* _matrix);
  void setOutputFileName(string);
  void serializeImportance(short, long, double);
  void updateSenderScore(long fromEdge, double sc);
  void updateReceiverScore(long fromEdge, double sc);
  void setPartitionConfigHandler(GraphPartitionConfigHandler*);

private:
  GraphPartitionConfigHandler* partConfHandler;
  EdgelistContainer* matrix;
  vector<double>* pagerankScore;
  vector<double>* tmpReceiverScore;
  vector<double>* tmpSenderScore;

  string outfile;
  long allNode_;
  double dump_;
  int actIter;
  int maxIter;

  log4cpp::Category* logger_;
};


#endif  // PAGERANK_NON_BLOCK_NODE_H_
