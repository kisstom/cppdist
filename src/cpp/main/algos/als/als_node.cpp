#include "als_node.h"

AlsNode::AlsNode(unordered_map<string, string>* params) {
  logger_ = &log4cpp::Category::getInstance(std::string("AlsNode"));
  lambda = util.stringToDouble((*params)["LAMBDA"]);
  maxIter = util.stringToInt((*params)["MAX_ITER"]);
  int numItem, numUser;
  numFeat = util.stringToInt((*params)["NUM_FEAT"]);
  numItem = util.stringToInt((*params)["NUM_ITEMS"]);
  numUser = util.stringToInt((*params)["NUM_USERS"]);


  featP = new FeatureMatrix(numUser, numFeat);
  featQ = new FeatureMatrix(numItem, numFeat);
  alsUtil = new AlsUtil(numFeat);

  userPartition = NULL;
  itemPartition = NULL;
  configHandler = NULL;

  actIter = 0;
  wantAdd = sizeof(long) + numFeat * sizeof(double);
}

void AlsNode::setAlsPartConfigHandler(AlsPartitionConfigHandler* _configHandler ) {
  configHandler = _configHandler;
}

void AlsNode::beforeIteration(string msg) {

}

bool AlsNode::afterIteration() {
  return (++actIter < maxIter);
}

void AlsNode::sender() {
  if (actIter % 2 == 0) {
    senderUserUpdate();
  } else {
    senderItemUpdate();
  }
}

void AlsNode::senderUserUpdate() {
  long numNeighbors;
  Entry e;

  for (long partitionNode = 0; partitionNode < userPartition->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = userPartition->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;
    alsUtil->solveOptimisation(featQ, userPartition, partitionNode, featP, lambda);

    mutex.lock();
    alsUtil->updateFeature(featP, partitionNode +
        configHandler->getUserMinNode(algo_->getSlaveIndex()));
    mutex.unlock();

    broadCastFeatVect(featP, partitionNode +
        configHandler->getUserMinNode(algo_->getSlaveIndex()));
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}

void AlsNode::senderItemUpdate() {
  long numNeighbors;
  Entry e;

  for (long partitionNode = 0; partitionNode < itemPartition->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = itemPartition->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;
    alsUtil->solveOptimisation(featP, itemPartition, partitionNode, featQ, lambda);

    mutex.lock();
    alsUtil->updateFeature(featQ, partitionNode +
        configHandler->getItemMinNode(algo_->getSlaveIndex()));
    mutex.unlock();

    broadCastFeatVect(featQ, partitionNode +
        configHandler->getItemMinNode(algo_->getSlaveIndex()));
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}

void AlsNode::updateUserFeats(long key, double * feats) {
  mutex.lock();
  for (int featI = 0; featI < featP->getFeatureSize(); ++featI) {
    featP->updateEntry(key, featI, *(feats + featI));
  }
  mutex.unlock();
}

void AlsNode::updateItemFeats(long key, double * feats) {
  mutex.lock();
  for (int featI = 0; featI < featQ->getFeatureSize(); ++featI) {
    featQ->updateEntry(key, featI, *(feats + featI));
  }
  mutex.unlock();
}

void AlsNode::final() {}

void AlsNode::sendFeatVectTo(FeatureMatrix* featMx, long id, short partI) {
  if (!senderBuffer_->canAdd(wantAdd, partI)) {
    senderBuffer_->emptyBuffer(partI);
  }

  senderBuffer_->setBreak(partI);
  senderBuffer_->store(partI, id);
  for (int i = 0; i < numFeat; ++i) {
    senderBuffer_->store(id, featMx->getEntry(id, i));
  }
}

void AlsNode::broadCastFeatVect(FeatureMatrix* featMx, long id) {
  for (short i = 0; i < algo_->getNumSlaves(); ++i) {
    if (i == algo_->getSlaveIndex()) continue;
    sendFeatVectTo(featMx, id, i);
  }
}

void AlsNode::setUserPartition(AdjacencyList<Entry>* _userPart) {
  userPartition = _userPart;
}

void AlsNode::setItemPartition(AdjacencyList<Entry>* _itemPart) {
  itemPartition = _itemPart;
}

AlsNode::~AlsNode() {
  delete featP;
  delete featQ;
  delete alsUtil;
}
