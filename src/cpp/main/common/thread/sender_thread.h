/*
 * sender_thread.h
 *
 *  Created on: 2014.02.03.
 *      Author: kisstom
 */

#ifndef SENDER_THREAD_H_
#define SENDER_THREAD_H_

#include "../../algos/algo_components/node.h"
#include "thread_manager.h"
#include <log4cpp/Category.hh>

class SenderThread : public ThreadManager{
public:
  SenderThread(Node*);
protected:
  void main();
private:
  Node* node_;
	log4cpp::Category* logger_;
};


#endif /* SENDER_THREAD_H_ */
