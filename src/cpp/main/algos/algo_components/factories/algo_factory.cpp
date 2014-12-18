#include "algo_factory.h"
#include "../algo_base/two_threaded_algo.h"
#include "../algo_base/three_threaded_algo.h"

AlgoBase* AlgoFactory::createAlgo(unordered_map<string, string>* params) {
  if (params->find("MULTI") != params->end()) {
     return new ThreeThreadedAlgo(params);
  }

  return new TwoThreadedAlgo(params);
}
