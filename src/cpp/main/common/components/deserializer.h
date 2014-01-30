/*
 * deserializer.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef DESERIALIZER_H_
#define DESERIALIZER_H_

class Deserializer {
public:
	virtual void update(short partindex) = 0;
	virtual int storeFromBinary(char*, unsigned) = 0;
	virtual ~Deserializer() {}
};


#endif /* DESERIALIZER_H_ */
