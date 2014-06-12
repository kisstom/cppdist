/*
 * psimrank_node.cpp
 *
 *  Created on: 2014.06.12.
 *      Author: kisstom
 */

#include "psimrank_node.h"

PSimrankNode::PSimrankNode() {
  logger_ = &log4cpp::Category::getInstance(std::string("PSimrankNode"));
  fpIndex_ = 0;
  pathIndex_ = 0;
  pathLen_ = 0;
  numFingerprints_ = 0;
  oddIter_ = true;
  aCoef_ = 0;
  bCoef_ = 0;
  modulo_ = 0;
}

PSimrankNode::PSimrankNode(short numFingerprints, short pathLen,
    int seed, GeneratorType genType, long num_nodes, long min_node, long nextMinNode) {
  logger_ = &log4cpp::Category::getInstance(std::string("SimrankOddEvenNode"));
  logger_->info("Next min node %ld",  nextMinNode);
  fpIndex_ = 0;
  pathIndex_ = 0;
  numFingerprints_ = numFingerprints;
  pathLen_ = pathLen;
  numNodes_ = num_nodes;
  minNode_ = min_node;
  nextMinNode_ = nextMinNode;
  matrix_ = NULL;
  edgeListbuilder_ = NULL;
  oddIter_ = true;
  aCoef_ = 0;
  bCoef_ = 0;
  modulo_ = 0;
}

PSimrankNode::~PSimrankNode() {
  for (int i = 0; i < (int) fingerprints_.size(); ++i) {
    for (list<long*>::iterator it = fingerprints_[i].begin(); it != fingerprints_[i].end(); ++it) {
      delete[] *it;
    }
  }

  for (int i = 0; i < (int) finishedPathes_.size(); ++i) {
    for (int j = 0; j < (int) finishedPathes_[i].size(); ++j) {
      delete[] finishedPathes_[i][j];
    }
  }

  if (matrix_) delete matrix_;
}

void PSimrankNode::beforeIteration(string msg) {
  if (oddIter_) {
    logger_->info("\nBefore iteration.\nIter odd(paratlan) iter");

  } else {
    logger_->info("\nBefore iteration.\nIter even(paros) iter");
  }
  logger_->info("Fingerprint %hd path %hd.", fpIndex_, pathIndex_);
}

long PSimrankNode::genEdge(long from) {
  int numNeighbors = matrix_->neighborhoodSize(from);
  if (numNeighbors <= 0) {
    return -1;
  }

  uint128_t hash;
  uint128_t min(ULLONG_MAX, ULLONG_MAX);
  long arg_min = -1;

  for (int i = 0; i < numNeighbors; ++i) {
    uint128_t ui128_node(matrix_->getEdgeAtPos(from, i));
    hash = (ui128_node * aCoef_ + bCoef_) % modulo_;

    if (hash < min) {
      min = hash;
      arg_min = matrix_->getEdgeAtPos(from, i);
    }

  }

  return arg_min;
}

void PSimrankNode::senderOdd() {
  logger_->info("Sender odd started.");
  long pathEnd;
  short partIndex;

  for (list<long*>::iterator it = fingerprints_[fpIndex_].begin();
      it != fingerprints_[fpIndex_].end(); ++it) {

    pathEnd = (*it)[pathIndex_];
    partIndex = algo_->getPartitionIndex(pathEnd);

    if (partIndex == partIndex_) {
      if (nextNodes_.find(pathEnd) == nextNodes_.end()) {
        nextNodes_[pathEnd] = genEdge(pathEnd);
      }

    } else {

      if (nextNodes_.find(pathEnd) == nextNodes_.end()) {
        // IMPORTANT ezt be kell rakni mert kulonben tobbszor elkuldom
        nextNodes_[pathEnd] = -2;
        serializeRequest(pathEnd, partIndex);
      }
    }

  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Sender odd finished.");
}


void PSimrankNode::senderEven() {
  logger_->info("Sender even started.");

  long nextNode;
  for (tr1::unordered_map<long, vector<short> >::iterator it =
      sendBack_.begin(); it != sendBack_.end(); ++it) {

    nextNodesMutex_.lock();
    if (nextNodes_.find(it->first) == nextNodes_.end()) {
      nextNodes_[it->first] = genEdge(it->first);
    }
    nextNodesMutex_.unlock();

    for (vector<short>::iterator jt = it->second.begin();
        jt != it->second.end(); ++jt) {
      nextNodesMutex_.lock();
      nextNode = nextNodes_[it->first];
      nextNodesMutex_.unlock();
      serializeAnswer(it->first, nextNode, *jt);
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Sender even finished.");
}


