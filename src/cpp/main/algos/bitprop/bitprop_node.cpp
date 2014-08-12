/*
 * bitprop_node.cpp
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#include "bitprop_node.h"
#include "random_bitvector_generator.h"
#include <cmath>

BitpropNode::BitpropNode(int _numCodingBytes, int _estIndex, double _epsilon, long _minnode) {
  randomVectorBits = NULL;
  aux = NULL;
  temp = NULL;
  matrix = NULL;
  numCodingBytes = _numCodingBytes;
  estIndex = _estIndex;
  epsilon = _epsilon;
  neighborhoodDistance = 0;
  failedEstimatedNodes = NULL;
  estimationHandler = NULL;
  edgeListBuilder = NULL;
  minnode = _minnode;
  logger = &log4cpp::Category::getInstance(std::string("BitpropNode"));
}

void BitpropNode::sender() {
  long numNeighbors = 0, outEdge = 0;
  int partIndex = 0;

  for (long partitionNode = 0; partitionNode < matrix->getNumberOfNodes(); ++partitionNode) {
    numNeighbors = matrix->neighborhoodSizePart(partitionNode);
    if (0.0 == numNeighbors) continue;

    for (long i = 0; i < numNeighbors; ++i) {
      outEdge = matrix->getEdgeAtPosPart(partitionNode, i);
      partIndex = algo_->getPartitionIndex(outEdge);

      //logger->info("my part %d target part %d", partIndex_, partIndex);
      if (partIndex == partIndex_) {
        mutex.lock();

        for (int byteIndex = 0; byteIndex < numCodingBytes; ++byteIndex) {
          *(temp + (outEdge - matrix->getMinnode()) * numCodingBytes + byteIndex) |=
              *(aux + partitionNode * numCodingBytes + byteIndex);
        }

        mutex.unlock();
      } else {
        serializeRandomBits(partitionNode, outEdge, partIndex);
      }
    }
  }

  algo_->sendAndSignal(partIndex_);
}

void BitpropNode::updateBits(long outEdge, unsigned char* bits) {
  mutex.lock();

  for (int byteIndex = 0; byteIndex < numCodingBytes; ++byteIndex) {
    *(temp + (outEdge - matrix->getMinnode()) * numCodingBytes + byteIndex) |=
        *(bits + byteIndex);
  }

  mutex.unlock();
}

void BitpropNode::setFailedEstimateNodes(std::vector<FailedEstimate>* _failedEstimatedNodes) {
  failedEstimatedNodes = _failedEstimatedNodes;
}

void BitpropNode::setEstimatonHandler(IEstimationHandler* _estimationHandler) {
  estimationHandler = _estimationHandler;
}

void BitpropNode::setContainer(EdgelistContainer* _container) {
  matrix = _container;
}

void BitpropNode::beforeIteration(string msg) {
  ++neighborhoodDistance;
  logger->info("Starting neighbor distance %hd", neighborhoodDistance);
}

bool BitpropNode::afterIteration() {
  updateBuffers();
  estimate();
  return true;
}

void BitpropNode::initFromMaster(string) {

}

void BitpropNode::initData(string partName) {
}

void BitpropNode::initBuffers() {
  aux = new unsigned char[matrix->getNumberOfNodes() * numCodingBytes];
  temp = new unsigned char[matrix->getNumberOfNodes() * numCodingBytes];

  memcpy(aux, randomVectorBits, matrix->getNumberOfNodes() * numCodingBytes);
  memcpy(temp, randomVectorBits, matrix->getNumberOfNodes() * numCodingBytes);
}


void BitpropNode::final() {

}

void BitpropNode::setRandomBits(unsigned char* rvb) {
  randomVectorBits = rvb;
}

void BitpropNode::updateBuffers() {
  memcpy(aux, temp, numCodingBytes * matrix->getNumberOfNodes());
  memcpy(temp, randomVectorBits, numCodingBytes * matrix->getNumberOfNodes());
}

void BitpropNode::serializeRandomBits(long from, long to, int partIndex) {
  logger->info("Serializing %ld %ld", from + matrix->getMinnode(), to);

  int shouldAdd = 1 + sizeof(long) + numCodingBytes;

  if (!senderBuffer_->canAdd(partIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(partIndex);
  }

  senderBuffer_->setBreak(partIndex);
  senderBuffer_->store(partIndex, to);
  senderBuffer_->store(partIndex, randomVectorBits + from * numCodingBytes, numCodingBytes);
}

int BitpropNode::numCodingOnes(long node) {
  int ones = 0;
  for (int i = 0; i < numCodingBytes; ++i) {
    unsigned char c = *(aux + node * numCodingBytes + i);
    for (int j = 0; j < 8; ++j) {
      ones += c % 2;
      c >>= 1;
    }
  }
  return ones;
}

void BitpropNode::findFirstLastIndices(std::vector<FailedEstimate>* failedEstimatedNodes, int* first, int* last) {
  bool foundStart = false;
  bool foundLast = false;

  for (int index = 0; index < (int) failedEstimatedNodes->size(); ++index) {
    if (!foundStart && (*failedEstimatedNodes)[index].neighborhoodDistance == neighborhoodDistance) {
      foundStart = true;
      *first = index;
    }

    if (foundStart && (*failedEstimatedNodes)[index].neighborhoodDistance != neighborhoodDistance) {
      foundLast = true;
      *last = index;
    }
  }

  if (!foundLast) *last = failedEstimatedNodes->size();
}

void BitpropNode::getEstimation(int ones, double* est, bool* sing) {
  if (ones == 8 * numCodingBytes) {
    *sing = true;
    *est = -1.0;
  } else {
    *sing = false;
    *est = log(1.0 - (double)ones / (8 * numCodingBytes)) / log(1.0 - epsilon);
  }
}

void BitpropNode::estimate() {
  int ones;
  double est;
  bool sing;
  long currentNode;
  FailedEstimate* failedEstimateNode = NULL;

  if (estIndex == 0) {
    for (long node = 0; node < matrix->getNumberOfNodes(); ++node) {
      currentNode = node + matrix->getMinnode();
      ones = numCodingOnes(node);

      getEstimation(ones, &est, &sing);

      if ((double)ones <= (1.0 - 1.0 / exp(1)) * 8 * numCodingBytes) {
        estimationHandler->acceptedEstimation(currentNode, est, neighborhoodDistance);
      } else {
        estimationHandler->failedEstimation(currentNode, est, neighborhoodDistance);
      }
    }
  } else {
    int firstIndex, lastIndex;
    findFirstLastIndices(failedEstimatedNodes, &firstIndex, &lastIndex);

    for (int index = firstIndex; index < lastIndex; ++index) {
      failedEstimateNode = &((*failedEstimatedNodes)[index]);
      ones = numCodingOnes(failedEstimateNode->node);
      currentNode = failedEstimateNode->node + matrix->getMinnode();

      getEstimation(ones, &est, &sing);

      if ((double)ones <= (1.0 - 1.0 / exp(1)) * 8 * numCodingBytes) {
        if (failedEstimateNode->est == -1.0) {
          estimationHandler->acceptedEstimation(currentNode, est, neighborhoodDistance);
        } else {
          estimationHandler->acceptedEstimation(currentNode, (est + failedEstimateNode->est)/2, neighborhoodDistance);
        }
      } else {
        if (sing) {
          estimationHandler->failedEstimation(currentNode, -1.0, neighborhoodDistance);
        } else {
          estimationHandler->failedEstimation(currentNode, est, neighborhoodDistance);
        }
      }
    }
  }
}
