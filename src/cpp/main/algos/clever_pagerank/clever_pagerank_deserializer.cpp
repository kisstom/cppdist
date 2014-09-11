/*
 * clever_pagerank_deserializer.cpp
 *
 *  Created on: 2014.09.09.
 *      Author: kisstom
 */

#include "clever_pagerank_deserializer.h"

CleverPagerankDeserializer::CleverPagerankDeserializer() {
  logger_ = &log4cpp::Category::getInstance(std::string("CleverPagerankDeserializer"));
  node_ = NULL;
}

void CleverPagerankDeserializer::update(short partindex) {
  node_->updateSelfScore(fromNode, imp);
}

int CleverPagerankDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&fromNode, buffer + stored, sizeof(long));
  stored += sizeof(long);
  memcpy(&imp, buffer + stored, sizeof(double));
  stored += sizeof(double);

  return stored;
}


bool CleverPagerankDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < sizeof(long) + sizeof(double)) {
    return false;
  }

  return true;
}

void CleverPagerankDeserializer::setNode(CleverPagerankNode* node) {
  node_ = node;
}

