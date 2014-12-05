/*
 * pagerank_deserializer.cpp
 *
 *  Created on: 2014.05.09.
 *      Author: kisstom
 */

#include "pagerank_deserializer.h"

PagerankDeserializer::PagerankDeserializer() {
  logger_ = &log4cpp::Category::getInstance(std::string("PagerankDeserializer"));
  messageCounter = 0;
  node_ = NULL;
}

PagerankDeserializer::~PagerankDeserializer() {
  logger_->info("Received %d messages.", messageCounter);
}

void PagerankDeserializer::update(short partindex) {
  node_->updateScore(to, val);
}

int PagerankDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&to, buffer + stored, sizeof(long));
  stored += sizeof(long);
  memcpy(&val, buffer + stored, sizeof(double));
  stored += sizeof(double);

  ++messageCounter;
  return stored;
}

bool PagerankDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < sizeof(long) + sizeof(double)) {
    return false;
  }

  return true;
}

void PagerankDeserializer::setNode(PagerankNode* node) {
  node_ = node;
}


