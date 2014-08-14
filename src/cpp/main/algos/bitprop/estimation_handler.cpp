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


EstimationHandler::EstimationHandler(string outdir, short neighborhoodSize, int partIndex) {
  logger = &log4cpp::Category::getInstance(std::string("EstimationHandler"));
  init(outdir, neighborhoodSize, partIndex);
  logger->info("Estimation handler inited.");
}

EstimationHandler::~EstimationHandler() {
  for (int i = 0; i < failedEstimationFiles.size(); ++i) {
    fclose(failedEstimationFiles[i]);
    fclose(outputFiles[i]);
  }
}


void EstimationHandler::init(string outdir, short neighborhoodSize, int partIndex) {
  logger->info("Initing estimation handler.");

  for (short i = 0; i < neighborhoodSize; ++i) {
     stringstream failed;
     failed << outdir << "failed_estimates_distance_" << i << "_part_" << partIndex;
     FILE* outputF = fopen(failed.str().c_str(), "w");
     if (NULL == outputF) {
       throw IOError("Failed opening failed estimate file: " + failed.str());
     }

     failedEstimationFiles.push_back(outputF);

     stringstream succed;
     succed << outdir << "estimates_distance_" << i << "_part_" << partIndex;
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

