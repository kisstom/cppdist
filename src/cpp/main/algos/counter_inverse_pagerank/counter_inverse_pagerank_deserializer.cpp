/*
 * counter_inverse_pagerank_deserializer.cpp
 *
 *  Created on: 2014.09.24.
 *      Author: kisstom
 */

#include "counter_inverse_pagerank_deserializer.h"

CounterInversePagerankDeserializer::CounterInversePagerankDeserializer() {
  logger_ = &log4cpp::Category::getInstance(std::string("CounterInversePagerankDeserializer"));
  node_ = NULL;
}

void CounterInversePagerankDeserializer::update(short partindex) {
  node_->update(partindex, importance);
}

int CounterInversePagerankDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&importance, buffer, sizeof(double));
  stored += sizeof(double);

  return stored;
}


bool CounterInversePagerankDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < sizeof(double)) {
    return false;
  }

  return true;
}

void CounterInversePagerankDeserializer::setNode(CounterInversePagerankNode* node) {
  node_ = node;
}


