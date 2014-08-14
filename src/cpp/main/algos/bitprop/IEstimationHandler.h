/*
 * IEstimationHandler.h
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

#ifndef IESTIMATIONHANDLER_H_
#define IESTIMATIONHANDLER_H_

#include "log4cpp/Category.hh"

class IEstimationHandler {
public:
  IEstimationHandler() {
    logger = &log4cpp::Category::getInstance(std::string("IEstimationHandler"));
  }

  virtual void failedEstimation(long node, double est, short atDistance) {
    logger->info("failed estimation %ld %lf %hd", node, est, atDistance);
  }

  virtual void acceptedEstimation(long node, double est, short atDistance) {
    logger->info("accepted estimation %ld %lf %hd", node, est, atDistance);
  }

  virtual ~IEstimationHandler() {}

private:
  log4cpp::Category* logger;
};


#endif /* IESTIMATIONHANDLER_H_ */
