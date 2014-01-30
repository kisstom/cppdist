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
	virtual void receiver() {}
	virtual void sender() {}
	virtual bool setUp() {return false;}
	virtual void run() {}
	virtual ~Runnable() {}
};


#endif /* RUNNABLE_H_ */
