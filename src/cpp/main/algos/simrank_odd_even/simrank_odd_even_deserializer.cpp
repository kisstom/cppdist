/*
 * simrank_odd_even_deserializer.cpp
 *
 *  Created on: 2014.01.13.
 *      Author: kisstom
 */

#include "simrank_odd_even_deserializer.h"

SimrankOddEvenDeserializer::SimrankOddEvenDeserializer() {
	logger_ = &log4cpp::Category::getInstance(std::string("SimrankOddEvenDeserializer"));
}

void SimrankOddEvenDeserializer::update(short partindex) {
	if (*oddIter_) {
    node_->storeRequest(from_, partindex);
	} else {
    node_->storeRequestedEdge(from_, to_);
	}
}

int SimrankOddEvenDeserializer::storeFromBinary(char* buffer, unsigned length) {
	bool shouldRead = checkReadable(buffer, length);
	if (!shouldRead) return -1;

	int stored = 0;
	if (*oddIter_) {
		memcpy(&from_, buffer + stored, sizeof(long));
		stored += sizeof(long);
	} else {
		memcpy(&from_, buffer + stored, sizeof(long));
		stored += sizeof(long);
		memcpy(&to_, buffer + stored, sizeof(long));
		stored += sizeof(long);
	}

	return stored;
}

bool SimrankOddEvenDeserializer::checkReadable(char* buffer, unsigned length) {
  if (*oddIter_) {
  	if (length < sizeof(long)){
  		return false;
  	}
  } else {
  	if (length < sizeof(long) * 2){
  		return false;
  	}
  }

  return true;
}

void SimrankOddEvenDeserializer::setNode(SimrankOddEvenNode* node) {
	node_ = node;
	oddIter_ = node->getOddIter();
}


