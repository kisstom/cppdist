/*
 * estimation_handler.h
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#ifndef ESTIMATION_HANDLER_H_
#define ESTIMATION_HANDLER_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

#include "IEstimationHandler.h"

class EstimationHandler : public IEstimationHandler {
public:
  EstimationHandler(string outdir, short neighborhoodSize);
  void init(string outdir, short neighborhoodSize);
  void failedEstimation(long node, double est, short atDistance);
  void acceptedEstimation(long node, double est, short atDistance);

private:
  vector<FILE*> outputFiles;
  vector<FILE*> failedEstimationFiles;
};


#endif /* ESTIMATION_HANDLER_H_ */
