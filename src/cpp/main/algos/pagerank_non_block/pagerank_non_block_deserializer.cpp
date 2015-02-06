#include "pagerank_non_block_deserializer.h"

PagerankNonBlockDeserializer::PagerankNonBlockDeserializer() {
  logger_ = &log4cpp::Category::getInstance(std::string("PagerankNonBlockDeserializer"));
  node_ = NULL;
  messageCounter = 0;
}

PagerankNonBlockDeserializer::~PagerankNonBlockDeserializer() {
  logger_->info("Received %d messages.", messageCounter);
}

void PagerankNonBlockDeserializer::update(short partindex) {
  node_->updateReceiverScore(to, val);
}

int PagerankNonBlockDeserializer::storeFromBinary(char* buffer, unsigned length) {
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

bool PagerankNonBlockDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < sizeof(long) + sizeof(double)) {
    return false;
  }

  return true;
}

void PagerankNonBlockDeserializer::setNode(PagerankNonBlockNode* node) {
  node_ = node;
}
