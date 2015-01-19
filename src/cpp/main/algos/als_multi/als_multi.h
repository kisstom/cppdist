#ifndef ALS_MULTI_H_
#define ALS_MULTI_H_

#include "../als/als_node.h"

class AlsMultiNode : public AlsNode {
public:
  AlsMultiNode(unordered_map<string, string>* params);
  void broadCastFeatVect(FeatureMatrix* featMx, long id);
  ~AlsMultiNode();
private:
  void setBroadCastIndex(unordered_map<string, string>* params);
  short broadcastIndex;
};

#endif  // ALS_MULTI_H_
