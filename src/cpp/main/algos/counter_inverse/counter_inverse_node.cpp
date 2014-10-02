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
  bounds = NULL;
  newComer = NULL;
  partitionBound = NULL;
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
        update(partIndex, partitionNode, outEdge);
      } else {
        serializeEdge(partIndex, partitionNode, outEdge);
      }
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}

void CounterInverseNode::update(short partIndex, long from, long to) {
  mutex.lock();
  if (from != (*newComer)[partIndex]) {
    ++(*counter)[partIndex];
    (*newComer)[partIndex] = from;
  }

  inversePartsEdges->push_back(InverseTriple((*counter)[partIndex] - 1,
      to - matrix->getMinnode(), partIndex));
  mutex.unlock();
}

void CounterInverseNode::serializeEdge(int bufferIndex, long from, long to) {
  int shouldAdd = 1 + sizeof(long) * 2;

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, from);
  senderBuffer_->store(bufferIndex, to);
}

void CounterInverseNode::beforeIteration(string msg) {}

bool CounterInverseNode::afterIteration() {
  std::sort(inversePartsEdges->begin(),
       inversePartsEdges->end(), InverseTriple::compare);
  determineBounds();
  return false;
}

void CounterInverseNode::setEdgeListContainer(EdgelistContainer* _matrix) {
  matrix = _matrix;
  inversePartsEdges = new vector<InverseTriple>();

}

void CounterInverseNode::setCounters(int numPart) {
  counter = new vector<long>(numPart, 0);
  newComer = new vector<long>(numPart, -1);
}

void CounterInverseNode::setOutputFile(string outputFile) {
  outfile = outputFile;
}

void CounterInverseNode::setPartitionBoundFile(string file) {
  partitionBoundFile = file;
}

void CounterInverseNode::determineBounds() {
  bounds = new vector<long>(algo_->getNumberOfPartitions() + 1, 0);
  for (int i = 0; i < (int) counter->size() + 1; ++i) {
    if (i  > 0) {
      (*bounds)[i] = (*bounds)[i - 1] + (*counter)[i - 1];
    }
  }
}

void CounterInverseNode::final() {
  FILE* partitionBound = fopen(partitionBoundFile.c_str(), "w");
  for (int i = 0; i < (int) bounds->size(); ++i) {
    fprintf(partitionBound, "%ld\n", (*bounds)[i]);
  }
  fclose(partitionBound);

  FILE* output = fopen(outfile.c_str(), "w");
  long prev = 0, act = -1, index = -1;

  for (long i = 0; i < (long) inversePartsEdges->size(); ++i) {
    act = (*inversePartsEdges)[i].to;
    index = (*inversePartsEdges)[i].count + (*bounds)[(*inversePartsEdges)[i].fromPartition];
    if (act != prev) {
      for (int emptyCount = 0; emptyCount < act - prev; ++emptyCount) {
        fprintf(output, "\n");
      }

      fprintf(output, "%ld", index);
    } else {
      fprintf(output, " %ld", index);
    }
    prev = act;
  }

  for (long i = 0; i < matrix->getNumberOfNodes() - (prev - matrix->getMinnode()) + 1; ++i) {
    fprintf(output, "\n");
  }

  fclose(output);
}


