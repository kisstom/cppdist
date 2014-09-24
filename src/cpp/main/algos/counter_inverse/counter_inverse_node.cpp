/*
 * counter_inverse_node.cpp
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#include "counter_inverse_node.h"
#include <algorithm>

CounterInverseNode::CounterInverseNode() {
  logger_ = &log4cpp::Category::getInstance(std::string("CounterInverseNode"));
  matrix = NULL;
  inversePartsEdges = NULL;
  counter = NULL;
}


void CounterInverseNode::sender() {
  long outEdge, numNeighbors;
  int partIndex;

  for (long partitionNode = 0; partitionNode < matrix->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = matrix->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;

    for (long i = 0; i < numNeighbors; ++i) {
      outEdge = matrix->getEdgeAtPosPart(partitionNode, i);
      partIndex = algo_->getPartitionIndex(outEdge);

      if (partIndex == partIndex_) {
        update(partIndex, outEdge);
      } else {
        serializeEdge(partIndex, outEdge);
      }
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}

void CounterInverseNode::update(short partIndex, long to) {
  mutex.lock();
  inversePartsEdges->push_back(InverseTriple((*counter)[partIndex],
      to - matrix->getMinnode(), partIndex));
  ++(*counter)[partIndex];
  mutex.unlock();
}

void CounterInverseNode::serializeEdge(int bufferIndex, long to) {
  int shouldAdd = 1 + sizeof(long);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, to);
}

void CounterInverseNode::beforeIteration(string msg) {}

bool CounterInverseNode::afterIteration() {
  return false;
}

void CounterInverseNode::setEdgeListContainer(EdgelistContainer* _matrix) {
  matrix = _matrix;
  inversePartsEdges = new vector<InverseTriple>();
  counter = new vector<long>(algo_->getNumberOfPartitions(), 0);
}

void CounterInverseNode::setOutputFile(string outputFile) {
  outfile = outputFile;
}

void CounterInverseNode::setPartitionBoundFile(string file) {
  partitionBoundFile = file;
}

vector<long> CounterInverseNode::determineBounds() {
  vector<long> bounds(algo_->getNumberOfPartitions() + 1, 0);
  for (int i = 0; i < (int) counter->size() + 1; ++i) {
    if (i  > 0) {
      bounds[i] = bounds[i - 1] + (*counter)[i - 1];
    }
  }

  return bounds;
}

void CounterInverseNode::final() {
  vector<long> bounds = determineBounds();

  FILE* partitionBound = fopen(partitionBoundFile.c_str(), "w");
  for (int i = 0; i < (int) bounds.size(); ++i) {
    fprintf(partitionBound, "%ld\n", bounds[i]);
  }
  fclose(partitionBound);

  std::sort(inversePartsEdges->begin(),
      inversePartsEdges->end(), InverseTriple::compare);

  FILE* output = fopen(outfile.c_str(), "w");
  long prev = -1, act = -1, index = -1;

  for (long i = 0; i < (long) inversePartsEdges->size(); ++i) {
    act = (*inversePartsEdges)[i].to;
    index = (*inversePartsEdges)[i].count + bounds[(*inversePartsEdges)[i].fromPartition];
    if (act != prev) {
      if (i != 0) {
        for (int emptyCount = 0; emptyCount < prev - act; ++emptyCount) {
          fprintf(output, "\n");
        }
      }

      fprintf(output, "%ld", index);
    }

    fprintf(output, " %ld", index);
    prev = act;
  }
  fclose(output);
}


