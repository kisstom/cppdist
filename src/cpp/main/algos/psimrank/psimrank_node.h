/*
 * psimrank_node.h
 *
 *  Created on: 2014.06.12.
 *      Author: kisstom
 */

#ifndef PSIMRANK_NODE_H_
#define PSIMRANK_NODE_H_

#include <limits.h>

#include <tr1/unordered_map>
#include <list>

#include "../../common/graph/edgelist_container.h"
#include "../../common/random/random.h"
#include "../../common/random/hash_pseudo_random.h"
#include "../../common/components/mutex.h"
#include "../../common/graph/edge_list_builder.h"
#include "../node.h"
#include "../algo.h"
#include "../../common/util/uint128_t.h"

using std::string;
using std::vector;
using std::tr1::unordered_map;

class PSimrankNode : public Node {
public:
  PSimrankNode();
  ~PSimrankNode();
  PSimrankNode(short numFingerprints, short pathLen,
      long num_nodes, long min_node, long nextMinNode);
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void senderOdd();
  void senderEven();
  void initFromMaster(string);
  void initData(string);
  void final();

  long genEdge(long from);
  void serializeRequest(long, short);
  void serializeAnswer(long from, long to, short bufferIndex);
  bool incrementIndices();
  void incrementPathes();
  void initStartForAll(long from, long to, long numnodes,
      short numFingerPrints);
  void storeRequest(long from, short partIndex);
  void storeRequestedEdge(long from, long to);
  int hashToEdgeIndex(int hash, int length);
  void initFingerprints();
  void initEdgeListContainer(string partName);

  void setFingerprints(vector<list<long*> >);
  void setMatrix(EdgelistContainer* matrix);
  void setNextNodes(unordered_map<long, long>);
  void setOutputFile(string fname);
  void setFingerPrintFile(string fpStartFname);
  void initFinishedPathes(vector<vector<long*> >);
  vector<list<long*> >* getPathes();
  vector<vector<long*> >* getFinishedPathes();
  bool* getOddIter() {return &oddIter_;}
  void setEdgeListBuilder(IEdgeListBuilder*);

private:
  Mutex nextNodesMutex_;
  unordered_map<long, long> nextNodes_;
  unordered_map<long, vector<short> > sendBack_;

  vector<list<long*> > fingerprints_;
  vector<vector<long*> > finishedPathes_;

  short numFingerprints_;
  short pathLen_;
  short fpIndex_;
  short pathIndex_;
  bool oddIter_;
  long numNodes_;
  long minNode_;
  long nextMinNode_;

  string fpStartFname_;
  string outFileName_;

  EdgelistContainer* matrix_;
  IEdgeListBuilder* edgeListbuilder_;
  long aCoef_, bCoef_, modulo_;
  log4cpp::Category* logger_;
};


#endif /* PSIMRANK_NODE_H_ */
