/*
 * estimation_handler.cpp
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#include "estimation_handler.h"
#include "../../common/util/util.h"
#include <cstdio>


EstimationHandler::EstimationHandler(unordered_map<string, string>* params) {
  Util util;
  util.checkParam(params, 2, "BASE_OUTDIR", "NEIGHBORHOOD_SIZE");
  //outputFiles = fopen(outputFileName.c_str(), "w");
  //failedEstimationFiles = fopen(failedEstimationFileName.c_str(), "w");
}

void EstimationHandler::failedEstimation(long node, double est, short atDistance) {
  fprintf(failedEstimationFiles[atDistance], "%ld %lf\n", node, est);
}

void EstimationHandler::acceptedEstimation(long node, double est, short atDistance) {
  fprintf(outputFiles[atDistance], "%ld %lf\n", node, est);
}

