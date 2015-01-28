#include "als_broadcast_node.h"
#include "../../common/components/socket/multicast_helper.h"

AlsBroadcastNode::AlsBroadcastNode(unordered_map<string, string>* params) : AlsNode(params) {
  logger_ = &log4cpp::Category::getInstance(std::string("AlsBroadcastNode"));
}

void AlsBroadcastNode::broadCastFeatVect(FeatureMatrix* featMx, long id) {
  //logger_->info("broadCastFeatVect");
  sendFeatVectTo(featMx, id, 0);
}


AlsBroadcastNode::~AlsBroadcastNode() {

}
