/*
 * runnable.h
 *
 *  Created on: 2013.08.12.
 *      Author: kisstom
 */

#ifndef RUNNABLE_H_
#define RUNNABLE_H_

class Runnable {
public:
	virtual bool setUp() = 0;
	virtual void run() = 0;
	virtual ~Runnable() {}
};


#endif /* RUNNABLE_H_ */
