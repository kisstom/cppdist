/*
 * IEstimationHandler.h
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

#ifndef IESTIMATIONHANDLER_H_
#define IESTIMATIONHANDLER_H_

class IEstimationHandler {
public:
  virtual void failedEstimation(long node, double est, short atDistance) {}
  virtual void acceptedEstimation(long node, double est, short atDistance) {}
  virtual ~IEstimationHandler() {}
};


#endif /* IESTIMATIONHANDLER_H_ */
