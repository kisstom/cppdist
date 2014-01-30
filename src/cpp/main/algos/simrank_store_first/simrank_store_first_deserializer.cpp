/*
 * simrank_store_first_serializer.cpp
 *
 *  Created on: 2013.11.06.
 *      Author: kisstom
 */

#include "simrank_store_first_deserializer.h"

SimrankStoreFirstDeserializer::SimrankStoreFirstDeserializer() {
	logger_ = &log4cpp::Category::getInstance(std::string("SimrankStoreFirstDeserializer"));
}

void SimrankStoreFirstDeserializer::update(short partindex) {
  if (type_ == '0') {
    node_->consumeRequest(from_, partindex);
  } else if (type_ == '1') {
  	node_->consumeAnswer(from_, to_);
  } else if (type_ == '2') {
  	//logger_->info("Receiving sender end from %hd", partindex);
  	node_->registerNodeFinished(partindex);
  }
}

int SimrankStoreFirstDeserializer::storeFromBinary(char* buffer, unsigned length) {
	bool shouldRead = checkReadable(buffer, length);
	if (!shouldRead) return -1;

	// the type char
	int stored = sizeof(char);
	if (type_ == '0') {
		memcpy(&from_, buffer + stored, sizeof(long));
		stored += sizeof(long);
	} else if (type_ == '1') {
		memcpy(&from_, buffer + stored, sizeof(long));
		stored += sizeof(long);
		memcpy(&to_, buffer + stored, sizeof(long));
		stored += sizeof(long);
	} else if (type_ == '2') {
    // nothing to do
	} else {
    // TODO error handling
		logger_->error("Unknown type of deserialization %c.", type_);
	}

	return stored;
}

bool SimrankStoreFirstDeserializer::checkReadable(char* buffer, unsigned length) {
  if (length < 1) return false;

  memcpy(&type_, buffer, sizeof(char));
  if (type_ == '0') {
  	if (length < sizeof(char) + sizeof(long)){
  		return false;
  	}
  } else if (type_ == '1') {
  	if (length < sizeof(char) + sizeof(long) * 2){
  		return false;
  	}
  } else if (type_ == '2') {
    return true;
  } else {
  	// TODO error handling
  	logger_->error("Unknown type of deserialization %c.", type_);
  }

  return true;
}

void SimrankStoreFirstDeserializer::setNode(SimrankStoreFirstNode* node) {
	node_ = node;
}

