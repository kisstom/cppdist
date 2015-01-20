#include "als_multi.h"
#include "../../common/components/socket/multicast_helper.h"

AlsMultiNode::AlsMultiNode(unordered_map<string, string>* params) : AlsNode(params) {
  logger_ = &log4cpp::Category::getInstance(std::string("AlsMultiNode"));
  setBroadCastIndex(params);
}

void AlsMultiNode::broadCastFeatVect(FeatureMatrix* featMx, long id) {
  //logger_->info("broadCastFeatVect");
  sendFeatVectTo(featMx, id, broadcastIndex);
}

void AlsMultiNode::setBroadCastIndex(unordered_map<string, string>* params) {
  int slaveIndex = util.stringToInt((*params)["SLAVE_INDEX"]);
  int numSlaves = util.stringToInt((*params)["NUM_SLAVES"]);

  set<short> everyButMe;
  for (short i = 0; i < numSlaves; ++i) {
    if (i != slaveIndex) everyButMe.insert(i);
  }

  bool foo;
  MulticastHelper helper(slaveIndex);
  broadcastIndex = helper.publishHashId(everyButMe, &foo);
}

AlsMultiNode::~AlsMultiNode() {

}
