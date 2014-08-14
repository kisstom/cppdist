/*
 * memory_estimation_handler.cpp
 *
 *  Created on: 2014.08.13.
 *      Author: kisstom
 */

#include "thread_safe_memory_estimation_handler.h"

ThreadSafeMemoryEstimationHandler::ThreadSafeMemoryEstimationHandler() {}


void ThreadSafeMemoryEstimationHandler::failedEstimation(long node, double est, short atDistance) {
  mutex.lock();
  failedEstimations[atDistance][node] = est;
  mutex.unlock();
}

void ThreadSafeMemoryEstimationHandler::acceptedEstimation(long node, double est, short atDistance) {
  mutex.lock();
  acceptedEstimations[atDistance][node] = est;
  mutex.unlock();
}
