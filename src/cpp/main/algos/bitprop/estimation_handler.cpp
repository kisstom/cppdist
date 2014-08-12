/*
 * estimation_handler.cpp
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#include "estimation_handler.h"
#include "../../common/util/util.h"
#include "../../common/util/ConnectionError.h"
#include <cstdio>


EstimationHandler::EstimationHandler(string outdir, short neighborhoodSize) {
  init(outdir, neighborhoodSize);
}

void EstimationHandler::init(string outdir, short neighborhoodSize) {
  failedEstimationFiles.resize(neighborhoodSize);
  outputFiles.resize(neighborhoodSize);

  for (short i = 0; i < neighborhoodSize; ++i) {
     stringstream failed;
     failed << outdir << "failed_estimates_distance_" << i;
     FILE* outputF = fopen(failed.str().c_str(), "w");
     if (NULL == outputF) {
       throw IOError("Failed opening failed estimate file: " + failed.str());
     }

     failedEstimationFiles.push_back(outputF);

     stringstream succed;
     succed << outdir << "estimates_distance_" << i;
     outputF = fopen(succed.str().c_str(), "w");

     if (NULL == outputF) {
       throw IOError("Failed opening failed estimate file: " + succed.str());
     }
     outputFiles.push_back(outputF);
  }
}

void EstimationHandler::failedEstimation(long node, double est, short atDistance) {
  fprintf(failedEstimationFiles[atDistance - 1], "%ld %lf\n", node, est);
}

void EstimationHandler::acceptedEstimation(long node, double est, short atDistance) {
  fprintf(outputFiles[atDistance - 1], "%ld %lf\n", node, est);
}

