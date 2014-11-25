/*
 * custom_non_block_deserializer.cpp
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#include "custom_multi_non_block_deserializer.h"

CustomMultiNonBlockDeserializer::CustomMultiNonBlockDeserializer() {
  logger_ = &log4cpp::Category::getInstance(std::string("CustomNonBlockDeserializer"));
  node_ = NULL;
}

void CustomMultiNonBlockDeserializer::update(short partindex) {
  node_->updateReceiverScore(fromNode, imp);
}

int CustomMultiNonBlockDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&fromNode, buffer + stored, sizeof(long));
  stored += sizeof(long);
  memcpy(&imp, buffer + stored, sizeof(double));
  stored += sizeof(double);

  return stored;
}


bool CustomMultiNonBlockDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < sizeof(long) + sizeof(double)) {
    return false;
  }

  return true;
}

void CustomMultiNonBlockDeserializer::setNode(CustomMultiNonBlockNode* node) {
  node_ = node;
}



