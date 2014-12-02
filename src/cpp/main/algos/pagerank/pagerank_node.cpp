/*
 * pagerank_node.cpp
 *
 *  Created on: 2014.04.15.
 *      Author: kisstom
 */

#include "pagerank_node.h"

PagerankNode::PagerankNode(int maxIter, long allNode, double dump) {
  logger_ = &log4cpp::Category::getInstance(std::string("PagerankNode"));
  this->maxIter = maxIter;
  this->allNode_ = allNode;
  this->dump_ = dump;
  actIter = 0;
  matrix_ = NULL;
  pagerankScore_ = NULL;
  tmpScore_ = NULL;
}

void PagerankNode::beforeIteration(string msg) {
  logger_->info("Starting iteration %d", actIter);
  for (long node = 0; node < matrix_->getNumberOfNodes(); ++node) {
    (*tmpScore_)[node] = 0.0;
  }
}

bool PagerankNode::afterIteration() {
  for (long node = 0; node < (long) tmpScore_->size(); ++node) {
    (*pagerankScore_)[node] = (*tmpScore_)[node] * (1.0 - dump_) + dump_ / allNode_;
    (*tmpScore_)[node] = 0.0;
  }
  return (++actIter < maxIter);
}

void PagerankNode::sender() {
  long outEdge, numNeighbors;
  int partIndex;

  for (long partitionNode = 0; partitionNode < matrix_->getNumberOfNodes(); ++partitionNode) {
    //logger_->info("sender: %d", partitionNode);
    numNeighbors = matrix_->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;

    double imp = (*pagerankScore_)[partitionNode] / numNeighbors;


    for (long i = 0; i < numNeighbors; ++i) {
      outEdge = matrix_->getEdgeAtPosPart(partitionNode, i);
      partIndex = algo_->getPartitionIndex(outEdge);

      if (partIndex == partIndex_) {
        tmpScoreMutex_.lock();
        (*tmpScore_)[outEdge - matrix_->getMinnode()] += imp;
        tmpScoreMutex_.unlock();
      } else {
        serializeImportance(partIndex, outEdge, imp);
      }
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}

void PagerankNode::final() {
  FILE* outf = fopen(outfile.c_str(), "w");
  if (NULL == outf) {
    logger_->error("Error opening file %s for writing.", outfile.c_str());
    return;
  }

  for (long node = 0; node < matrix_->getNumberOfNodes(); ++node) {
    fprintf(outf, "%ld %.10lf\n", node + matrix_->getMinnode(), (*pagerankScore_)[node]);
  }

  fclose(outf);
}

void PagerankNode::updateScore(long outEdge, double sc) {
  tmpScoreMutex_.lock();
  (*tmpScore_)[outEdge - matrix_->getMinnode()] += sc;
  tmpScoreMutex_.unlock();
}

void PagerankNode::serializeImportance(int bufferIndex, long outNode, double importance) {
  //logger_->info("Beginning of serializeImportance");
  int shouldAdd = 1 + sizeof(long) + sizeof(double);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    //logger_->info("Urites %d-nek", bufferIndex);
    senderBuffer_->emptyBuffer(bufferIndex);
    //logger_->info("Urites vege");
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, outNode);
  senderBuffer_->store(bufferIndex, importance);
  //logger_->info("End of serializeImportance");
}

void PagerankNode::setEdgeListContainer(EdgelistContainer* matrix) {
  matrix_ = matrix;
  double ini = 1.0 / allNode_;
  pagerankScore_ = new vector<double>(matrix->getNumberOfNodes(), ini);
  tmpScore_ = new vector<double>(matrix->getNumberOfNodes(), 0.0);
}

void PagerankNode::setOutputFile(string outputFile) {
  outfile = outputFile;
}
