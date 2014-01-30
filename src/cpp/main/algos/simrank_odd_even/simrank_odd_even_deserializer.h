/*
 * simrank_odd_even_deserializer.h
 *
 *  Created on: 2014.01.13.
 *      Author: kisstom
 */

#ifndef SIMRANK_ODD_EVEN_DESERIALIZER_H_
#define SIMRANK_ODD_EVEN_DESERIALIZER_H_

#include "../../common/components/deserializer.h"
#include "simrank_odd_even_node.h"

class SimrankOddEvenDeserializer: public Deserializer {
public:
	SimrankOddEvenDeserializer();
	void update(short partindex);
	int storeFromBinary(char*, unsigned);
	bool checkReadable(char* buffer, unsigned length);
	void setNode(SimrankOddEvenNode*);
private:
	long from_;
	long to_;
	bool* oddIter_;
	SimrankOddEvenNode* node_;
	log4cpp::Category* logger_;
};


#endif /* SIMRANK_ODD_EVEN_DESERIALIZER_H_ */
