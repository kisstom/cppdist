/*
 * psimrank_master.cpp
 *
 *  Created on: 2014.06.12.
 *      Author: kisstom
 */

#include "psimrank_master.h"

PSimrankMaster::PSimrankMaster() {
  logger_ = &log4cpp::Category::getInstance(std::string("PSimrankMaster"));
  logger_->info("PSimrankMaster inited.");
  iterNum_ = 0;
}

void PSimrankMaster::setRandomGenerator(PSimrankRandomGeneratorIFace* rgen) {
  randomGenerator_ = rgen;
}

void PSimrankMaster::init() {
  setPrime();
}

bool PSimrankMaster::nextIter() {
  logger_->info("nextIter");

  if (iterNum_++ % 2 == 1) {
    InnerMaster::nextIter();
    return true;
  }

  long aCoef = generateRandomCoeff();
  long bCoef = generateRandomCoeff();

  char msg[1024];
  sprintf(msg, "%ld %ld", aCoef, bCoef);
  logger_->info("Sending msg %s.", msg);
  master_->sendMessageForAllNodes(msg);

  return true;
}

void PSimrankMaster::addInfoForNodes(char* ss) {
  long long_prime = mpz_get_si(prime_);
  sprintf(ss, " %ld", long_prime);
}

void PSimrankMaster::setPrime() {
  randomGenerator_->setPrime(&prime_, master_->getNumNodes());
}

long PSimrankMaster::generateRandomCoeff() {
  return randomGenerator_->generateRandomCoeff();
}

