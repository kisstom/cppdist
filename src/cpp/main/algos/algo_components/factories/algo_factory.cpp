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
  AlgoFactoryHelper helper;
  AlgoBase* retval = helper.initAlgo(params);

  //vector<long>* partitionMinNodes = readSlaveConfig(params);
  //retval->setPartitionMinNodes(partitionMinNodes);

  return retval;
}

/*vector<long>*  AlgoFactory::readSlaveConfig(unordered_map<string, string>* params) {
  string cfg = (*params)["LOCAL_SLAVE_CONFIG"];
  FILE* slavery_par = fopen(cfg.c_str(), "r");
  if (slavery_par == NULL) {
    logger_->error("Error opening %s in algo factory.", cfg.c_str());
    return NULL;
  }

  vector<long>* partitionMinNodes = new vector<long>();
  int num_slaves = atoi((*params)["NUM_SLAVES"].c_str());
  int minNode = -1;

  for (int i = 0; i < num_slaves; ++i) {
    fscanf(slavery_par,"%*d %*s %*ld %*ld %ld", &minNode);
    partitionMinNodes->push_back(minNode);
  }

  return partitionMinNodes;
}*/
