#ifndef ALS_NODE_H_
#define ALS_NODE_H_

#include "feature_matrix.h"
#include "als_util.h"
#include "partitioner.h"
#include "../../common/graph/adjacency_list.h"
#include "../../common/graph/entry.h"
#include "../../common/util/util.h"
#include "../../common/components/mutex.h"
#include "../algo_components/node.h"
#include "log4cpp/Category.hh"

class AlsNode : public Node {
public:
  friend class AlsDeserializer;
  AlsNode(unordered_map<string, string>* params);

  virtual void beforeIteration(string msg);
  virtual bool afterIteration();
  virtual void sender();
  virtual void initFromMaster(string) {}
  virtual void final();

  void broadCastFeatVect(FeatureMatrix* featMx, long id);
  void sendFeatVectTo(FeatureMatrix* featMx, long id, short partI);
  void updateUserFeats(long key, double*);
  void updateItemFeats(long key, double*);
  void senderUserUpdate();
  void senderItemUpdate();

  ~AlsNode();
private:
  AdjacencyList<Entry>* userPartition;
  AdjacencyList<Entry>* itemPartition;
  FeatureMatrix* featP;
  FeatureMatrix* featQ;
  Partitioner* itemPartitioner;
  Partitioner* userPartitioner;

  AlsUtil* alsUtil;
  Util util;
  Mutex mutex;

  string outfile;
  double lambda;
  int actIter;
  int maxIter;
  int numFeat;
  int wantAdd;

  log4cpp::Category* logger_;
};


#endif  // ALS_NODE_H_
