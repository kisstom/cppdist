#include "algo_factory.h"
#include "../algo_base/two_threaded_algo.h"
#include "../algo_base/three_threaded_algo.h"
#include "algo_factory_helper.h"
#include <cstdio>
#include <cstdlib>

AlgoFactory::AlgoFactory() {
  logger_ = &log4cpp::Category::getInstance(std::string("AlgoFactory"));
}

AlgoBase* AlgoFactory::createAlgo(unordered_map<string, string>* params) {
  logger_->info("Creating algo.");
  AlgoFactoryHelper helper;
  AlgoBase* retval = helper.initAlgo(params);
  logger_->info("Finished creating algo.");
  return retval;
}
