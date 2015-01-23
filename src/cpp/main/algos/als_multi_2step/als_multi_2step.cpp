#include "als_multi_2step.h"
#include "../../common/components/socket/multicast_helper.h"

AlsMulti2Step::AlsMulti2Step(unordered_map<string, string>* params) {
  logger_ = &log4cpp::Category::getInstance(std::string("AlsMulti2Step"));
  lambda = util.stringToDouble((*params)["LAMBDA"]);
  maxIter = util.stringToInt((*params)["MAX_ITER"]);
  int numItem, numUser;
  numFeat = util.stringToInt((*params)["NUM_FEAT"]);
  numItem = util.stringToInt((*params)["NUM_ITEMS"]);
  numUser = util.stringToInt((*params)["NUM_USERS"]);
  double defIniVal = util.stringToDouble((*params)["DEF_INI_VAL"]);

  featP = new FeatureMatrix(numUser, numFeat, defIniVal, true);
  featQ = new FeatureMatrix(numItem, numFeat, defIniVal, true);
  alsUtil = new AlsUtil(numFeat);

  userPartition = NULL;
  itemPartition = NULL;
  configHandler = NULL;

  actIter = 0;
  innerIter = -1;
  wantAdd = sizeof(long) + numFeat * sizeof(double);

  setBroadCastIndex(params);
}

int AlsMulti2Step::getWantAdd() {
  return wantAdd;
}

void AlsMulti2Step::setBroadCastIndex(unordered_map<string, string>* params) {
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

void AlsMulti2Step::setUserFeatFile(string _fname) {
  userFeatfile = _fname;
}

void AlsMulti2Step::setItemFeatFile(string _fname) {
  itemFeatfile = _fname;
}

void AlsMulti2Step::setAlsPartConfigHandler(AlsPartitionConfigHandler* _configHandler ) {
  configHandler = _configHandler;
}

void AlsMulti2Step::beforeIteration(string msg) {
  logger_->info("Starting inner iter %d at iteration %d", innerIter, actIter);
}

bool AlsMulti2Step::afterIteration() {
  logger_->info("Finished inner it %d at iteration %d", innerIter, actIter);

  ++innerIter;
  if (innerIter < algo_->getNumSlaves()) {
    return true;
  }

  innerIter = -1;
  return (++actIter < maxIter);
}

void AlsMulti2Step::sender() {
  if (actIter % 2 == 0) {
    if (innerIter == -1) {
      computeUserUpdate();
      return;
    }

    if (innerIter == algo_->getSlaveIndex()) {
      userFeatBroadcast();
    } else {
      skipSender();
    }
  } else {
    if (innerIter == -1) {
      computeItemUpdate();
      return;
    }

    if (innerIter == algo_->getSlaveIndex()) {
      itemFeatBroadcast();
    } else {
      skipSender();
    }
  }
}

void AlsMulti2Step::userFeatBroadcast() {
  for (long partitionNode = 0; partitionNode < userPartition->getNumberOfNodes();
      ++partitionNode) {
    broadCastFeatVect(featP, partitionNode +
        configHandler->getUserMinNode(algo_->getSlaveIndex()));
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished userFeatBroadcast.");
}

void AlsMulti2Step::itemFeatBroadcast() {
  for (long partitionNode = 0; partitionNode < itemPartition->getNumberOfNodes();
      ++partitionNode) {
    broadCastFeatVect(featQ, partitionNode +
        configHandler->getItemMinNode(algo_->getSlaveIndex()));
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished itemFeatBroadcast.");
}

void AlsMulti2Step::computeUserUpdate() {
  long numNeighbors;
  Entry e;

  for (long partitionNode = 0; partitionNode < userPartition->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = userPartition->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;
    alsUtil->solveOptimisation(featQ, userPartition, partitionNode, lambda);

    alsUtil->updateFeature(featP, partitionNode +
        configHandler->getUserMinNode(algo_->getSlaveIndex()));
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished computeUserUpdate.");
}

void AlsMulti2Step::computeItemUpdate() {
  long numNeighbors;
  Entry e;

  for (long partitionNode = 0; partitionNode < itemPartition->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = itemPartition->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;
    alsUtil->solveOptimisation(featP, itemPartition, partitionNode, lambda);

    alsUtil->updateFeature(featQ, partitionNode +
        configHandler->getItemMinNode(algo_->getSlaveIndex()));
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished computeItemUpdate.");
}

void AlsMulti2Step::updateUserFeats(long key, double * feats) {
  for (int featI = 0; featI < featP->getFeatureSize(); ++featI) {
    featP->updateEntry(key, featI, *(feats + featI));
  }
}

void AlsMulti2Step::updateItemFeats(long key, double * feats) {
  for (int featI = 0; featI < featQ->getFeatureSize(); ++featI) {
    featQ->updateEntry(key, featI, *(feats + featI));
  }
}

void AlsMulti2Step::flushToFile(FeatureMatrix* featMx, string fname) {
  FILE* out = fopen(fname.c_str(), "w");
  if (NULL == out) {
    logger_->error("Error opening file %s", fname.c_str());
    return;
  }

  for (int i = 0; i < featMx->getRowSize(); ++i) {
    for (int j = 0; j < featMx->getFeatureSize(); ++j) {
      fprintf(out, "%lf ", featMx->getEntry(i, j));
    }
    fprintf(out, "\n");
  }

  fclose(out);
}

void AlsMulti2Step::final() {
  if (algo_->getSlaveIndex() == 0) {
    flushToFile(featP, userFeatfile);
    flushToFile(featQ, itemFeatfile);
  }
}

void AlsMulti2Step::sendFeatVectTo(FeatureMatrix* featMx, long id, short partI) {
  if (!senderBuffer_->canAdd(partI, wantAdd + 1)) {
    senderBuffer_->emptyBuffer(partI);
  }

  senderBuffer_->setBreak(partI);
  senderBuffer_->store(partI, id);
  for (int i = 0; i < numFeat; ++i) {
    senderBuffer_->store(partI, featMx->getEntry(id, i));
  }
}

void AlsMulti2Step::broadCastFeatVect(FeatureMatrix* featMx, long id) {
  sendFeatVectTo(featMx, id, broadcastIndex);
}

void AlsMulti2Step::setUserPartition(AdjacencyList<Entry>* _userPart) {
  userPartition = _userPart;
}

void AlsMulti2Step::setItemPartition(AdjacencyList<Entry>* _itemPart) {
  itemPartition = _itemPart;
}

void AlsMulti2Step::skipSender() {
  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished skipSender.");
}

bool AlsMulti2Step::isReceivingUpdates() {
  if (innerIter == -1) return false;
  if (innerIter == algo_->getSlaveIndex()) {
    return false;
  }
  return true;
}

bool AlsMulti2Step::isUserIteration() {
  return actIter % 2 == 0;
}

AlsMulti2Step::~AlsMulti2Step() {
  delete featP;
  delete featQ;
  delete alsUtil;
  if (userPartition) delete userPartition;
  if (itemPartition) delete itemPartition;
}
