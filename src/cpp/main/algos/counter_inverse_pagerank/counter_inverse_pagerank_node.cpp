/*
 * counter_inverse_pagerank_node.cpp
 *
 *  Created on: 2014.09.24.
 *      Author: kisstom
 */

#include "counter_inverse_pagerank_node.h"

CounterInversePagerankNode::CounterInversePagerankNode(
    long _allnode, double _dump, int _maxIter) {
  logger_ = &log4cpp::Category::getInstance(std::string("CounterInversePagerankNode"));

  allNode = _allnode;
  dump = _dump;
  actIter = 0;
  maxIter = _maxIter;

  pointerToCounters = NULL;
  outPartitionIndices = NULL;
  incomingPageranks = NULL;
  pagerankScore = NULL;
  counter = NULL;
  partitionBound = NULL;
}

void CounterInversePagerankNode::sender() {
  long numNeighbors;
  int partIndex;

  for (long partitionNode = 0;
      partitionNode < outPartitionIndices->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = outPartitionIndices->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;

    double imp = (*pagerankScore)[partitionNode] / numNeighbors;

    for (long i = 0; i < numNeighbors; ++i) {
      partIndex = outPartitionIndices->getEdgeAtPosPart(partitionNode, i);

      if (partIndex == partIndex_) {
        update(partIndex, imp);
      } else {
        serializeImportance(partIndex, imp);
      }
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}

void CounterInversePagerankNode::update(short partIndex, double imp) {
  mutex.lock();
  long index = (*counter)[partIndex] + (*partitionBound)[partIndex];
  ++(*counter)[partIndex];
  (*incomingPageranks)[index] = imp;
  mutex.unlock();
}

void CounterInversePagerankNode::serializeImportance(int bufferIndex, double importance) {
  int shouldAdd = 1 + sizeof(double);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, importance);
}

void CounterInversePagerankNode::beforeIteration(string msg) {
  for (int i = 0; i < (int) counter->size(); ++i) {
    (*counter)[i] = 0;
  }
}

void CounterInversePagerankNode::updateWithIncomingPr() {
  long numNeighbors;
  long index;

  for (long partitionNode = 0;
      partitionNode < pointerToCounters->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = pointerToCounters->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;

    for (long i = 0; i < numNeighbors; ++i) {
      if (i == 0) {
        (*pagerankScore)[partitionNode] = 0;
      }

      index = pointerToCounters->getEdgeAtPosPart(partitionNode, i);
      (*pagerankScore)[partitionNode] += (*incomingPageranks)[index];
    }

    (*pagerankScore)[partitionNode] = (*pagerankScore)[partitionNode] * (1 - dump)
        + dump / allNode;
  }
}

void CounterInversePagerankNode::initFromMaster(string) {
  pagerankScore = new vector<double>(algo_->getNumberOfPartitionNodes(), 0.0);
}

bool CounterInversePagerankNode::afterIteration() {
  updateWithIncomingPr();
  return (++actIter < maxIter);
}

void CounterInversePagerankNode::final() {
  FILE* outf = fopen(outfile.c_str(), "w");
  if (NULL == outf) {
    logger_->error("Error opening file %s for writing.", outfile.c_str());
    return;
  }

  for (long node = 0; node < pagerankScore->size(); ++node) {
    fprintf(outf, "%ld %.10lf\n", node + algo_->getMinnode(), (*pagerankScore)[node]);
  }

  fclose(outf);
}

void CounterInversePagerankNode::setOutputFile(string outputFile) {
  outfile = outputFile;
}

void CounterInversePagerankNode::setPointerToCounters(EdgelistContainer* _pointerToCounters) {
  pointerToCounters = _pointerToCounters;
}

void CounterInversePagerankNode::setOutpartitionIndices(EdgelistContainer* _outPartitionIndices) {
  outPartitionIndices = _outPartitionIndices;
}

void CounterInversePagerankNode::setPartitionBound(vector<long>* _partitionBound) {
  partitionBound = _partitionBound;
  long numberOfCounters = (*partitionBound)[(int) partitionBound->size()];
  incomingPageranks = new vector<double>(numberOfCounters, 0.0);
}

void CounterInversePagerankNode::readPartitionBouns(string fname) {
  FILE* file = fopen(fname.c_str(), "r");
  if (file == NULL) {
    logger_->error("Error opening file %s for writing.", fname.c_str());
    return;
  }

  partitionBound = new vector<long>();
  long bound;
  while (fscanf(file, "%ld\n", &bound) != EOF) {
    partitionBound->push_back(bound);
  }

  fclose(file);
}