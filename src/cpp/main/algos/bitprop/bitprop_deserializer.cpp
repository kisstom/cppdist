/*
 * bitprop_deserializer.cpp
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#include "bitprop_deserializer.h"

BitpropDeserializer::BitpropDeserializer(int _numCodingBytes) {
  numCodingBytes = _numCodingBytes;
  codingBytes = new unsigned char[numCodingBytes];
  logger = &log4cpp::Category::getInstance(std::string("BitpropDeserializer"));
}

void BitpropDeserializer::update(short partindex) {
  node->updateBits(to, codingBytes);
}

int BitpropDeserializer::storeFromBinary(char* buffer, unsigned length) {
  bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

  int stored = 0;
  memcpy(&to, buffer + stored, sizeof(long));

  stored += sizeof(long);
  memcpy(codingBytes, buffer + stored, numCodingBytes);
  stored += numCodingBytes;

  logger->info("To %ld receiving char %u", to, *codingBytes);

  return stored;
}

bool BitpropDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < sizeof(long) + numCodingBytes) {
    return false;
  }

  return true;
}

void BitpropDeserializer::setNode(BitpropNode* _node) {
  node = _node;
}
