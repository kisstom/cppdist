/*
 * simple_mock_deserializer.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "simple_mock_deserializer.h"

SimpleMockDeserializer::SimpleMockDeserializer() {
	logger_ = &log4cpp::Category::getInstance(std::string("Algo"));
}

void SimpleMockDeserializer::update(short pi) {
  node_->update(actInt_, actLong_);
}

int SimpleMockDeserializer::storeFromBinary(char* buffer, unsigned length) {
	if (length < sizeof(long) + sizeof(int)) return -1;
	int stored = 0;

	memcpy(&actInt_, buffer + stored, sizeof(int));
	stored += sizeof(int);
	memcpy(&actLong_, buffer + stored, sizeof(long));
	stored += sizeof(long);

	return stored;
}

void SimpleMockDeserializer::setNode(SimpleMockNode* n) {
	node_ = n;
}

