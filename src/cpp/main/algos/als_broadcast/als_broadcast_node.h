#ifndef ALS_BROADCAST_NODE_H_
#define ALS_BROADCAST_NODE_H_

#include "../als/als_node.h"

class AlsBroadcastNode : public AlsNode {
public:
  AlsBroadcastNode(unordered_map<string, string>* params);
  void broadCastFeatVect(FeatureMatrix* featMx, long id);
  ~AlsBroadcastNode();
};


#endif  // ALS_BROADCAST_NODE_H_
