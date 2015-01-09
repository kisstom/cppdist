#include "als_deserializer.h"
#include <cstdlib>

AlsDeserializer::AlsDeserializer(int _numFeat) {
  feats = new double[_numFeat];
  logger_ = &log4cpp::Category::getInstance(std::string("AlsDeserializer"));
  numFeat = _numFeat;
  key = -1;
  node_ = NULL;
}

void AlsDeserializer::update(short partindex) {
  if (node_->actIter % 2 == 0) {
    node_->updateUserFeats(key, feats);
  } else {
    node_->updateItemFeats(key, feats);
  }
}

int AlsDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&key, buffer + stored, sizeof(long));
  stored += sizeof(long);

  for (int i = 0; i < numFeat; ++i) {
    memcpy(feats + i, buffer + stored, sizeof(double));
    stored += sizeof(double);
  }

  return stored;
}

bool AlsDeserializer::checkReadable(char* buffer, unsigned length) {
  return length >= (unsigned) node_->wantAdd;
}

void AlsDeserializer::setNode(AlsNode* node) {
  node_ = node;
}

AlsDeserializer::~AlsDeserializer() {
  delete[] feats;
}
