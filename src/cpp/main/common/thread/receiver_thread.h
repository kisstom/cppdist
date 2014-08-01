/*
 * receiver_thread.h
 *
 *  Created on: 2014.02.03.
 *      Author: kisstom
 */

#ifndef RECEIVER_THREAD_H_
#define RECEIVER_THREAD_H_

#include "../../algos/algo_components/algo.h"
#include "thread_manager.h"
#include <log4cpp/Category.hh>

class ReceiverThread : public ThreadManager {
public:
	ReceiverThread(Algo*);
protected:
  void main();
private:
  Algo* algo_;
	log4cpp::Category* logger_;
};


#endif /* RECEIVER_THREAD_H_ */
