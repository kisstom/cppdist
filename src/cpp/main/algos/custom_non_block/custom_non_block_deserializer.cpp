/*
 * custom_non_block_deserializer.cpp
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#include "custom_non_block_deserializer.h"

CustomNonBlockDeserializer::CustomNonBlockDeserializer() {
  logger_ = &log4cpp::Category::getInstance(std::string("CustomNonBlockDeserializer"));
  node_ = NULL;
  messageCounter = 0;
}

CustomNonBlockDeserializer::~CustomNonBlockDeserializer() {
  logger_->info("Received %d messages.", messageCounter);
}

void CustomNonBlockDeserializer::update(short partindex) {
  node_->updateReceiverScore(fromNode, imp);
}

int CustomNonBlockDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&fromNode, buffer + stored, sizeof(long));
  stored += sizeof(long);
  memcpy(&imp, buffer + stored, sizeof(double));
  stored += sizeof(double);

  ++messageCounter;
  return stored;
}


bool CustomNonBlockDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < sizeof(long) + sizeof(double)) {
    return false;
  }

  return true;
}

void CustomNonBlockDeserializer::setNode(CustomNonBlockNode* node) {
  node_ = node;
}



