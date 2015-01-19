#ifndef ALS_NODE_H_
#define ALS_NODE_H_

#include "feature_matrix.h"
#include "als_util.h"
//#include "partitioner.h"
#include "../algo_components/factories/als_partition_config_handler.h"
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

  void setUserPartition(AdjacencyList<Entry>*);
  void setItemPartition(AdjacencyList<Entry>*);
  void setAlsPartConfigHandler(AlsPartitionConfigHandler*);

  virtual void broadCastFeatVect(FeatureMatrix* featMx, long id);
  void sendFeatVectTo(FeatureMatrix* featMx, long id, short partI);
  void updateUserFeats(long key, double*);
  void updateItemFeats(long key, double*);
  void senderUserUpdate();
  void senderItemUpdate();
  void setUserFeatFile(string fname);
  void setItemFeatFile(string fname);
  void flushToFile(FeatureMatrix*, string fname);
  ~AlsNode();
protected:
  AdjacencyList<Entry>* userPartition;
  AdjacencyList<Entry>* itemPartition;
  FeatureMatrix* featP;
  FeatureMatrix* featQ;
  AlsPartitionConfigHandler* configHandler;

  AlsUtil* alsUtil;
  Util util;
  Mutex mutex;

  string itemFeatfile;
  string userFeatfile;
  double lambda;
  int actIter;
  int maxIter;
  int numFeat;
  int wantAdd;

  log4cpp::Category* logger_;
};


#endif  // ALS_NODE_H_
