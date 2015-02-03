#include "algo_factory_helper.h"
#include "../algo_base/two_threaded_algo.h"
#include "../algo_base/three_threaded_algo.h"

AlgoBase* AlgoFactoryHelper::initAlgo(unordered_map<string, string>* params) {
  if (params->find("MULTI") != params->end()) {
    if ((*params)["MULTI"].compare("MULTICAST") == 0 ||
        (*params)["MULTI"].compare("BROADCAST") == 0) {
      return new ThreeThreadedAlgo(params);
    } else {
      throw std::exception();
    }
  }

  return new TwoThreadedAlgo(params);
}
