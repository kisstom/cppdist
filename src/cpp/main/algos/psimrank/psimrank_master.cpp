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

bool PSimrankMaster::nextIter() {
  logger_->info("nextIter");

  if (iterNum_++ % 2 == 1) {
    InnerMaster::nextIter();
    return true;
  }

  long randomPrime = generateRandomPrime();
  long randomAdd = generateRandomAdd();

  char msg[1024];
  sprintf(msg, "%ld %ld", randomPrime, randomAdd);
  logger_->info("Sending msg %s.", msg);
  master_->sendMessageForAllNodes(msg);

  return true;
}

long PSimrankMaster::generateRandomPrime() {
  return randomGenerator_->generateRandomPrime(master_->getNumNodes());
}

long PSimrankMaster::generateRandomAdd() {
  //logger_->info("nn %ld", master_->getNumNodes());
  return randomGenerator_->generateRandomAdd(master_->getNumNodes());
}
