/*
 * mock_deserializer.h
 *
 *  Created on: 2014.01.29.
 *      Author: kisstom
 */

#ifndef MOCK_DESERIALIZER_H_
#define MOCK_DESERIALIZER_H_

#include "../../main/common/components/deserializer.h"

class MockDeserializer : public Deserializer {
	MOCK_METHOD1(update, void(short));
	MOCK_METHOD2(storeFromBinary, int(char*, unsigned));
};


#endif /* MOCK_DESERIALIZER_H_ */
