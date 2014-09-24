/*
 * counter_inverse_deserializer.cpp
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#include "counter_inverse_deserializer.h"

CounterInverseDeserializer::CounterInverseDeserializer() {
  logger_ = &log4cpp::Category::getInstance(std::string("CounterInverseDeserializer"));
  node_ = NULL;
}

void CounterInverseDeserializer::update(short partindex) {
  node_->update(partindex, to);
}

int CounterInverseDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&to, buffer, sizeof(long));
  stored += sizeof(long);

  return stored;
}


bool CounterInverseDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < sizeof(long)) {
    return false;
  }

  return true;
}

void CounterInverseDeserializer::setNode(CounterInverseNode* node) {
  node_ = node;
}


