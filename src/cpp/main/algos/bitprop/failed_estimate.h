/*
 * node_need_estimate.h
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#ifndef FAILED_ESTIMATE_H_
#define FAILED_ESTIMATE_H_

struct FailedEstimate {
public:
  FailedEstimate(double e, long n, short d):
    est(e), node(n), neighborhoodDistance(d) {}

  double est;
  long node;
  short neighborhoodDistance;
};


#endif /* FAILED_ESTIMATE_H_ */
