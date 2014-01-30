/*
 * simrank_update_deserializer.h
 *
 *  Created on: 2013.08.08.
 *      Author: kisstom
 */

#ifndef SIMRANK_UPDATE_DESERIALIZER_H_
#define SIMRANK_UPDATE_DESERIALIZER_H_

#include "../../common/old/random_walk.h"
#include "simrank_update_node.h"
#include "log4cpp/Category.hh"

//class SimrankUpdateNode;

class SimrankUpdateDeserializer : public Deserializer {
public:
	SimrankUpdateDeserializer();
	int storeFromBinary(char*, unsigned);
	void update(short pi);
  unsigned getBufferSize(int);
  bool checkReadable(char* buffer, unsigned);
  void setNode(SimrankUpdateNode*);

	RandomWalk buffer_;
	SimrankUpdateNode* node_;
	int bufferSize_;
private:
	log4cpp::Category* logger_;
};


#endif /* SIMRANK_UPDATE_DESERIALIZER_H_ */
