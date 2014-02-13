/*
 * simrank_thread.cpp
 *
 *  Created on: 2014.02.12.
 *      Author: mrita
 */

#include "simrank_thread.h"

SimrankThread::SimrankThread(Simrank* simrank_, short fpmin_, short fpmax_, short pathlength_) {
  simrank = simrank_;
  fpmin = fpmin_;
  fpmax = fpmax_;
  pathlength = pathlength_;
  logger = &log4cpp::Category::getInstance(std::string("SimrankThread"));
}

void SimrankThread::main() {
  simrank->getFingerprint(fpmin,fpmax,pathlength);
}

