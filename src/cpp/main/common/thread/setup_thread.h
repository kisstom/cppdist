/*
 * setup_thread.h
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#ifndef SETUP_THREAD_H_
#define SETUP_THREAD_H_

#include "../../algos/algo_components/runnable.h"
#include "thread_manager.h"
#include <log4cpp/Category.hh>

class SetupThread : public ThreadManager {
public:
  SetupThread(Runnable*);
protected:
  void main();
private:
  Runnable* runnable_;
  log4cpp::Category* logger_;
};


#endif /* SETUP_THREAD_H_ */
