/*
 * master_thread.h
 *
 *  Created on: 2014.02.03.
 *      Author: kisstom
 */

#ifndef MAIN_THREAD_H_
#define MAIN_THREAD_H_

#include "../../algos/algo_components/runnable.h"
#include "thread_manager.h"
#include <log4cpp/Category.hh>

class MainThread : public ThreadManager {
public:
	MainThread(Runnable*);
  protected:
    void main();
  private:
    Runnable* runnable_;
  	log4cpp::Category* logger_;
};


#endif /* MAIN_THREAD_H_ */
