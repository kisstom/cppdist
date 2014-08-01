/*
 * simple_mock_deserializer.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef SIMPLE_MOCK_DESERIALIZER_H_
#define SIMPLE_MOCK_DESERIALIZER_H_

#include "simple_mock_node.h"
#include "log4cpp/Category.hh"

class SimpleMockDeserializer : public Deserializer {
public:
	SimpleMockDeserializer();
	void update(short);
	int storeFromBinary(char*, unsigned);
	void setNode(SimpleMockNode* n);
private:
	int actInt_;
	long actLong_;
	SimpleMockNode* node_;
	log4cpp::Category* logger_;
};


#endif /* SIMPLE_MOCK_DESERIALIZER_H_ */
