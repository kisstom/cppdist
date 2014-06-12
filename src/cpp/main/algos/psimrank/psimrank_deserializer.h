/*
 * simrank_odd_even_deserializer.h
 *
 *  Created on: 2014.01.13.
 *      Author: kisstom
 */

#ifndef PSIMRANK_DESERIALIZER_H
#define PSIMRANK_DESERIALIZER_H

#include "../../common/components/deserializer.h"
#include "psimrank_node.h"

class PSimrankDeserializer: public Deserializer {
public:
	PSimrankDeserializer();
	void update(short partindex);
	int storeFromBinary(char*, unsigned);
	bool checkReadable(char* buffer, unsigned length);
	void setNode(PSimrankNode*);
private:
	long from_;
	long to_;
	bool* oddIter_;
	PSimrankNode* node_;
	log4cpp::Category* logger_;
};


#endif /* PSIMRANK_DESERIALIZER_H */
