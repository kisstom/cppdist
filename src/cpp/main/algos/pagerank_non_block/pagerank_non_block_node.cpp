#include "pagerank_non_block_node.h"

PagerankNonBlockNode::PagerankNonBlockNode(long _allnode, long _minnode, double _dump, int _maxIter) {
  logger_ = &log4cpp::Category::getInstance(std::string("PagerankNode"));
  this->maxIter = _maxIter;
  this->allNode_ = _allnode;
  this->dump_ = _dump;
  actIter = 0;
  matrix = NULL;
  pagerankScore = NULL;
  tmpSenderScore = NULL;
  tmpReceiverScore = NULL;
  partConfHandler = NULL;
}

void PagerankNonBlockNode::setPartitionConfigHandler(GraphPartitionConfigHandler* configHandler) {
  partConfHandler = configHandler;
}

void PagerankNonBlockNode::beforeIteration(string msg) {
  logger_->info("Starting iteration %d", actIter);
  for (long node = 0; node < matrix->getNumberOfNodes(); ++node) {
    (*tmpSenderScore)[node] = 0.0;
    (*tmpReceiverScore)[node] = 0.0;
  }
}

bool PagerankNonBlockNode::afterIteration() {
  logger_->info("After iteration started.");
  for (long node = 0; node < (long) tmpSenderScore->size(); ++node) {
    (*pagerankScore)[node] = ((*tmpSenderScore)[node] + (*tmpReceiverScore)[node]) *
        (1.0 - dump_) + dump_ / allNode_;
  }
  return (++actIter < maxIter);
}


void PagerankNonBlockNode::sender() {
  long outEdge, numNeighbors;
  int partIndex;

  for (long partitionNode = 0; partitionNode < matrix->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = matrix->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;

    double imp = (*pagerankScore)[partitionNode] / numNeighbors;

    for (long i = 0; i < numNeighbors; ++i) {
      outEdge = matrix->getEdgeAtPosPart(partitionNode, i);
      partIndex = partConfHandler->getPartitionIndex(outEdge);

      if (partIndex == partIndex_) {
        updateSenderScore(outEdge, imp);
      } else {
        serializeImportance(partIndex, outEdge, imp);
      }
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}

void PagerankNonBlockNode::final() {
  FILE* outf = fopen(outfile.c_str(), "w");
  if (NULL == outf) {
    logger_->error("Error opening file %s for writing.", outfile.c_str());
    return;
  }

  for (long node = 0; node < matrix->getNumberOfNodes(); ++node) {
    fprintf(outf, "%ld %.10lf\n", node + matrix->getMinnode(), (*pagerankScore)[node]);
  }

  fclose(outf);
}

void PagerankNonBlockNode::updateSenderScore(long outEdge, double sc) {
  (*tmpSenderScore)[outEdge - matrix->getMinnode()] += sc;
}

void PagerankNonBlockNode::updateReceiverScore(long outEdge, double sc) {
  (*tmpReceiverScore)[outEdge - matrix->getMinnode()] += sc;
}

void PagerankNonBlockNode::serializeImportance(short bufferIndex, long outNode, double importance) {
  int shouldAdd = 1 + sizeof(long) + sizeof(double);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, outNode);
  senderBuffer_->store(bufferIndex, importance);
}

void PagerankNonBlockNode::setEdgeListContainer(EdgelistContainer* _matrix) {
  matrix = _matrix;
  double ini = 1.0 / allNode_;
  pagerankScore = new vector<double>(matrix->getNumberOfNodes(), ini);
  tmpSenderScore = new vector<double>(matrix->getNumberOfNodes(), 0.0);
  tmpReceiverScore = new vector<double>(matrix->getNumberOfNodes(), 0.0);
}

void PagerankNonBlockNode::setOutputFileName(string outputFile) {
  outfile = outputFile;
}
