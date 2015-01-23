#ifndef ALS_MULTI_2STEP_H_
#define ALS_MULTI_2STEP_H_

#include "../als/feature_matrix.h"
#include "../als/als_util.h"
#include "../algo_components/factories/als_partition_config_handler.h"
#include "../../common/graph/adjacency_list.h"
#include "../../common/graph/entry.h"
#include "../../common/util/util.h"
#include "../algo_components/node.h"
#include "log4cpp/Category.hh"

class AlsMulti2Step : public Node {
public:
  AlsMulti2Step(unordered_map<string, string>* params);

  virtual void beforeIteration(string msg);
  virtual bool afterIteration();
  virtual void sender();
  virtual void initFromMaster(string) {}
  virtual void final();

  void setUserPartition(AdjacencyList<Entry>*);
  void setItemPartition(AdjacencyList<Entry>*);
  void setAlsPartConfigHandler(AlsPartitionConfigHandler*);
  void setBroadCastIndex(unordered_map<string, string>* params);

  virtual void broadCastFeatVect(FeatureMatrix* featMx, long id);
  void sendFeatVectTo(FeatureMatrix* featMx, long id, short partI);
  void updateUserFeats(long key, double*);
  void updateItemFeats(long key, double*);

  void computeUserUpdate();
  void computeItemUpdate();
  void userFeatBroadcast();
  void itemFeatBroadcast();
  void skipSender();
  bool isReceivingUpdates();
  bool isUserIteration();
  int getWantAdd();

  void setUserFeatFile(string fname);
  void setItemFeatFile(string fname);
  void flushToFile(FeatureMatrix*, string fname);

  ~AlsMulti2Step();
protected:
  AdjacencyList<Entry>* userPartition;
  AdjacencyList<Entry>* itemPartition;
  FeatureMatrix* featP;
  FeatureMatrix* featQ;
  AlsPartitionConfigHandler* configHandler;

  AlsUtil* alsUtil;
  Util util;

  string itemFeatfile;
  string userFeatfile;
  double lambda;
  int actIter;
  int maxIter;
  int numFeat;
  int wantAdd;
  short broadcastIndex;
  int innerIter;
  int numBroadCasts;

  log4cpp::Category* logger_;
};


#endif  // ALS_MULTI_2STEP_H_
