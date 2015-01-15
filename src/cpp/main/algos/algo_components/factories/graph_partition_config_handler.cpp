#include "graph_partition_config_handler.h"
#include "../../../common/util/util.h"

GraphPartitionConfigHandler::GraphPartitionConfigHandler() {
  logger_ = &log4cpp::Category::getInstance(std::string("GraphSlaveConfigReader"));;
  partitionMinNodes = NULL;
  partitionNumNodes = NULL;
}

GraphPartitionConfigHandler::~GraphPartitionConfigHandler() {
  if (partitionMinNodes) {
    delete partitionMinNodes;
    delete partitionNumNodes;
  }
}

int GraphPartitionConfigHandler::getPartitionIndex(long key) {
  for (int i = partitionMinNodes->size() - 1; i >= 0; --i) {
    if ((*partitionMinNodes)[i] <= key) return i;
  }
  return 0;
}

void GraphPartitionConfigHandler::readSlaveConfig(string cfg, int numSlaves) {
  logger_->info("Reading slave config.");

  FILE* slavery_par = fopen(cfg.c_str(), "r");
  if (slavery_par == NULL) {
    logger_->error("Error opening %s in algo factory.", cfg.c_str());
  }

  partitionMinNodes = new vector<long>();
  partitionNumNodes = new vector<long>();
  long minNode = -1;
  long numNode;

  for (int i = 0; i < numSlaves; ++i) {
    fscanf(slavery_par,"%ld %*ld %ld", &numNode, &minNode);
    partitionMinNodes->push_back(minNode);
    partitionNumNodes->push_back(numNode);
  }

  fclose(slavery_par);
}

long GraphPartitionConfigHandler::getMinNode(int index) {
  return (*partitionMinNodes)[index];
}

long GraphPartitionConfigHandler::getNextMinNode(int index) {
  ++index;
  if (index >= (int) partitionNumNodes->size()) {
    return (*partitionMinNodes)[(int) partitionMinNodes->size() - 1] +
        (*partitionNumNodes)[(int) partitionNumNodes->size() - 1];
  }

  return (*partitionMinNodes)[index];
}

long GraphPartitionConfigHandler::getNumNode(int index) {
  return (*partitionNumNodes)[index];
}
