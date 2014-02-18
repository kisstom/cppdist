/*
 * simrank_thread.h
 *
 *  Created on: 2014.02.12.
 *      Author: mrita
 */

#ifndef SIMRANK_THREAD_H_
#define SIMRANK_THREAD_H_

#include "../../common/thread/thread_manager.h"
#include <log4cpp/Category.hh>
#include "../simrank_one_computer/simrank.h"

class SimrankThread : public ThreadManager {
public:
  SimrankThread(Simrank* simrank_, short fpmin_, short fpmax_, short pathlength_);
protected:
  void main();
private:
  Simrank* simrank;
  short fpmin;
  short fpmax;
  short pathlength;
  log4cpp::Category* logger;
 


};


#endif /* SIMRANK_THREAD_H_ */
