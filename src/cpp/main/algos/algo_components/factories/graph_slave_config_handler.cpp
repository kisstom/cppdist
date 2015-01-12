#include "graph_slave_config_reader.h"
#include "../../../common/util/util.h"

GraphSlaveConfigReader::GraphSlaveConfigReader() {
  logger_ = &log4cpp::Category::getInstance(std::string("GraphSlaveConfigReader"));;
  partitionMinNodes = NULL;
  partitionNumNodes = NULL;
}

GraphSlaveConfigReader::~GraphSlaveConfigReader() {
  if (partitionMinNodes) {
    delete partitionMinNodes;
    delete partitionNumNodes;
  }
}

void GraphSlaveConfigReader::readSlaveConfig(unordered_map<string, string>* params) {
  string cfg = (*params)["LOCAL_SLAVE_CONFIG"];
  FILE* slavery_par = fopen(cfg.c_str(), "r");
  if (slavery_par == NULL) {
    logger_->error("Error opening %s in algo factory.", cfg.c_str());
  }

  partitionMinNodes = new vector<long>();
  partitionNumNodes = new vector<long>();
  int num_slaves = atoi((*params)["NUM_SLAVES"].c_str());
  int minNode = -1;
  long numNode;

  for (int i = 0; i < num_slaves; ++i) {
    fscanf(slavery_par,"%*d %*s %ld %*ld %ld", &numNode, &minNode);
    partitionMinNodes->push_back(minNode);
    partitionNumNodes->push_back(numNode);
  }

  fclose(slavery_par);
}

long GraphSlaveConfigReader::getMinNodeFromParams(unordered_map<string, string>* params) {
  return (*partitionMinNodes)[util.stringToInt((*params)["SLAVE_INDEX"])];
}

long GraphSlaveConfigReader::getNextMinNodeFromParams(unordered_map<string, string>* params) {
  int index = util.stringToInt((*params)["SLAVE_INDEX"]) + 1;
  if (index >= (int) partitionNumNodes->size()) {
    return partitionMinNodes[(int) partitionMinNodes->size() - 1] +
        partitionNumNodes[(int) partitionNumNodes->size() - 1];
  }

  return (*partitionMinNodes)[index];
}

long GraphSlaveConfigReader::getNumNodeFromParams(unordered_map<string, string>* params) {
  return (*partitionNumNodes)[util.stringToInt((*params)["SLAVE_INDEX"])];
}
