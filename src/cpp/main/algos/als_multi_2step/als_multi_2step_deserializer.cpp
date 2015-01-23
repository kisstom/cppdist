#include "als_multi_2step_deserializer.h"
#include <cstdlib>

AlsMulti2StepDeserializer::AlsMulti2StepDeserializer(int _numFeat) {
  feats = new double[_numFeat];
  logger_ = &log4cpp::Category::getInstance(std::string("AlsMulti2StepDeserializer"));
  numFeat = _numFeat;
  key = -1;
  node_ = NULL;
  messageCounter = 0;
}

void AlsMulti2StepDeserializer::update(short partindex) {
  if (node_->isUserIteration()) {
    if (node_->isReceivingUpdates()) {
      node_->updateUserFeats(key, feats);
    }
  } else {
    if (node_->isReceivingUpdates()) {
      node_->updateItemFeats(key, feats);
    }
  }
}

int AlsMulti2StepDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&key, buffer + stored, sizeof(long));
  stored += sizeof(long);

  for (int i = 0; i < numFeat; ++i) {
    memcpy(feats + i, buffer + stored, sizeof(double));
    stored += sizeof(double);
  }

  ++messageCounter;
  return stored;
}

bool AlsMulti2StepDeserializer::checkReadable(char* buffer, unsigned length) {
  return length >= (unsigned) node_->getWantAdd();
}

void AlsMulti2StepDeserializer::setNode(AlsMulti2Step* node) {
  node_ = node;
}

AlsMulti2StepDeserializer::~AlsMulti2StepDeserializer() {
  logger_->info("Received %d messages.", messageCounter);
  delete[] feats;
}
