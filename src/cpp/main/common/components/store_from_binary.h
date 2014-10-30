/*
 * store_from_binary.h
 *
 *  Created on: 2013.08.08.
 *      Author: kisstom
 */

#ifndef STORE_FROM_BINARY_H_
#define STORE_FROM_BINARY_H_

#include <gtest/gtest_prod.h>
#include <vector>
#include "deserializer.h"
#include "serializer.h"

#include "log4cpp/Category.hh"

using std::vector;

class StoreFromBinary {
public:
	StoreFromBinary();
	virtual ~StoreFromBinary();
	virtual void setDeserializer(Deserializer*);
	virtual void resizeSocketNum(int);
  virtual int getSocketSize();
  virtual void setBufferCapacity(int);
  virtual bool storeFromIndex(int);
  virtual char* getEndOfBufferAt(int);
  vector<unsigned> remains_size_;
  vector<char*> receiver_remains_;
  int getRemainsSize(int socketIndex);
private:
	Serializer serializer_;
	Deserializer* deserializer_;
	log4cpp::Category* logger_;

	FRIEND_TEST(SimpleMockTestSetup, testSetup);
};


#endif /* STORE_FROM_BINARY_H_ */
