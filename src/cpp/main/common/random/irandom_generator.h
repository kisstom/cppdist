/*
 * irandom_generator.h
 *
 *  Created on: 2014.01.15.
 *      Author: kisstom
 */

#ifndef IRANDOM_GENERATOR_H_
#define IRANDOM_GENERATOR_H_

class IRandomGenerator {
public:
	virtual double uniRand(long node) = 0;
	virtual ~IRandomGenerator() {}
};


#endif /* IRANDOM_GENERATOR_H_ */
