/*
 * simrank_store_first_serializer.h
 *
 *  Created on: 2013.11.06.
 *      Author: kisstom
 */

#ifndef SIMRANK_STORE_FIRST_DESERIALIZER_H_
#define SIMRANK_STORE_FIRST_DESERIALIZER_H_

//#include <stdio.h>
#include <string.h>
#include "simrank_store_first_node.h"
#include "../../common/components/deserializer.h"
#include <log4cpp/Category.hh>

class SimrankStoreFirstDeserializer : public Deserializer {
public:
	SimrankStoreFirstDeserializer();
	void update(short partindex);
	int storeFromBinary(char*, unsigned);
	bool checkReadable(char* buffer, unsigned length);
	void setNode(SimrankStoreFirstNode*);
private:
	char type_;
	long from_;
	long to_;
  SimrankStoreFirstNode* node_;
  log4cpp::Category* logger_;
};


#endif /* SIMRANK_STORE_FIRST_DESERIALIZER_H_ */
