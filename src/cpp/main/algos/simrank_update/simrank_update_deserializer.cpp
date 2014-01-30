/*
 * simrank_update_deserializer.cpp
 *
 *  Created on: 2013.08.08.
 *      Author: kisstom
 */

#include "simrank_update_deserializer.h"
#include "simrank_update_node.h"
#include <stdio.h>
#include <string.h>


SimrankUpdateDeserializer::SimrankUpdateDeserializer() {
	logger_ = &log4cpp::Category::getInstance(std::string("SimrankUpdateDeserializer"));
	node_ = NULL;
}

bool SimrankUpdateDeserializer::checkReadable(char* buffer, unsigned length) {
	if (length < 2) return false;
  short size;

  memcpy(&size, buffer, sizeof(short));
  if (size <= 0) return false;

  if (getBufferSize(size) <= length) return true;

  return false;
}

int SimrankUpdateDeserializer::storeFromBinary(char* buffer, unsigned length) {
	bool shouldRead = checkReadable(buffer, length);
  if (!shouldRead) return -1;

	int stored = 0;
	memcpy(&buffer_.edge_num, buffer + stored, sizeof(short));

	stored += sizeof(short);
	for (int i = 0; i <= buffer_.edge_num; ++i) {
		memcpy(&buffer_.buf[i], buffer + stored, sizeof(long));
		stored += sizeof(long);
	}
	memcpy(&buffer_.fp_index, buffer + stored, sizeof(short));
	stored += sizeof(short);
  return stored;
}

void SimrankUpdateDeserializer::update(short pi) {
  node_->update(&buffer_);
}

unsigned SimrankUpdateDeserializer::getBufferSize(int numEdges) {
	return (numEdges + 1) * sizeof(long) + 2 * sizeof(short);
}

void SimrankUpdateDeserializer::setNode(SimrankUpdateNode* node) {
	node_ = node;
}
