/*
 * store_from_binary.cpp
 *
 *  Created on: 2013.08.08.
 *      Author: kisstom
 */

#include "store_from_binary.h"
#include <string.h>

StoreFromBinary::StoreFromBinary() {
	logger_ = &log4cpp::Category::getInstance(std::string("StoreFromBinary"));
	deserializer_ = NULL;
}

StoreFromBinary::~StoreFromBinary() {
	for (int i = 0; i < (int) receiver_remains_.size(); ++i) {
		delete[] receiver_remains_[i];
	}
}

void StoreFromBinary::setDeserializer(Deserializer* deserializer) {
	deserializer_ = deserializer;
}

void StoreFromBinary::resizeSocketNum(int size) {
	receiver_remains_.resize(size);
	remains_size_.resize(size);
}

int StoreFromBinary::getSocketSize() {
	return receiver_remains_.size();
}

void StoreFromBinary::setBufferCapacity(int c) {
	for (int i = 0; i < (int) receiver_remains_.size(); ++i) {
		receiver_remains_[i] = new char[c];
		remains_size_[i] = 0;
	}
}

bool StoreFromBinary::storeFromIndex(int socket_index) {
	unsigned stored = 0;
	int success;

  while (true) {

  	if (remains_size_[socket_index] == stored) {
  		if (stored > 0) {
  			memcpy(receiver_remains_[socket_index], receiver_remains_[socket_index] + stored,
  					remains_size_[socket_index] - stored );
  			remains_size_[socket_index] -= stored;
  		}
  		return true;
  	}

  	if (remains_size_[socket_index] == stored + 1) {
  		bool cont = true;
  		if (!serializer_.hasNext(receiver_remains_[socket_index] + stored)) {
  			cont = false;
  		}

  		if (stored > 0) {
  			memcpy(receiver_remains_[socket_index], receiver_remains_[socket_index] + stored,
  					remains_size_[socket_index] - stored );
  			remains_size_[socket_index] -= stored;
  		}

  		if (!cont) {
  			remains_size_[socket_index] = 0;
  		}

  		return cont;
  	}

    // +1 means ignoring seperator
  	logger_->info("deser %d %d", stored + 1, remains_size_[socket_index] - stored - 1);
    success = deserializer_->storeFromBinary(
    		receiver_remains_[socket_index] + stored + 1, remains_size_[socket_index] - stored - 1);

    // could not read any byte
    if (success <= 0) {
    	if (stored > 0) {
    		memcpy(receiver_remains_[socket_index], receiver_remains_[socket_index] + stored,
    				remains_size_[socket_index] - stored );
    		remains_size_[socket_index] -= stored;
    	}
    	return true;
    }

    stored += 1 + success;
  	deserializer_->update(socket_index);
  }
}

int StoreFromBinary::getRemainsSize(int socketIndex) {
	return remains_size_[socketIndex];
}

char* StoreFromBinary::getEndOfBufferAt(int socket_index) {
	return receiver_remains_[socket_index] + remains_size_[socket_index];
}
