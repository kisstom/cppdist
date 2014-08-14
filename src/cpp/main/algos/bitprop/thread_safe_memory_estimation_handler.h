/*
 * memory_estimation_handler.h
 *
 *  Created on: 2014.08.13.
 *      Author: kisstom
 */

#ifndef THREAD_SAFE_MEMORY_ESTIMATION_HANDLER_H_
#define THREAD_SAFE_MEMORY_ESTIMATION_HANDLER_H_

#include <tr1/unordered_map>
#include "IEstimationHandler.h"
#include "../../common/components/mutex.h"

using std::tr1::unordered_map;

class ThreadSafeMemoryEstimationHandler : public IEstimationHandler {
public:
  ThreadSafeMemoryEstimationHandler();
  void failedEstimation(long node, double est, short atDistance);
  void acceptedEstimation(long node, double est, short atDistance);

//private:
  unordered_map<short, unordered_map<long, double> > failedEstimations;
  unordered_map<short, unordered_map<long, double> > acceptedEstimations;

private:
  Mutex mutex;
};


#endif /* THREAD_SAFE_MEMORY_ESTIMATION_HANDLER_H_ */
