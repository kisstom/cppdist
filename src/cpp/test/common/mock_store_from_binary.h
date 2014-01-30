/*
 * mock_store_from_binary.h
 *
 *  Created on: 2013.08.16.
 *      Author: kisstom
 */

#ifndef MOCK_STORE_FROM_BINARY_H_
#define MOCK_STORE_FROM_BINARY_H_

#include "gmock/gmock.h"
//#include ""

class MockStoreFromBinary : public StoreFromBinary {
public:
	MOCK_METHOD1(setDeserializer, void(Deserializer*));
	MOCK_METHOD1(resizeSocketNum, void(int));
	MOCK_METHOD0(getSocketSize, int());
	MOCK_METHOD1(setBufferCapacity, void(int));
	MOCK_METHOD1(storeFromIndex, bool(int));
	MOCK_METHOD1(getEndOfBufferAt, char*(int));
};


#endif /* MOCK_STORE_FROM_BINARY_H_ */
