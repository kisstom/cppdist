/*
 * clever_pagerank.cpp
 *
 *  Created on: 2014.09.09.
 *      Author: kisstom
 */

#include "clever_pagerank_node.h"

 CleverPagerankNode::CleverPagerankNode(string _outfile, long _allnode,
      long _minnode, double _dump, int _actIter, int _maxIter) {
   logger_ = &log4cpp::Category::getInstance(std::string("CleverPagerankNode"));
   outfile = _outfile;
   allNode_ = _allnode;
   minNode_ = _minnode;
   dump_ = _dump;
   actIter = _actIter;
   maxIter = _maxIter;

   outPartitions = NULL;
   inverseNodeBounds = NULL;
   inverseOutEdges = NULL;
   numNeighbors = NULL;
   pagerankScore_ = NULL;
   tmpScore_ = NULL;
 }

void CleverPagerankNode::sender() {
  long origNode = minNode_ - 1, start, end;

  for (long partitionNode = 0; partitionNode < (long) outPartitions->size(); ++partitionNode) {
    ++origNode;
    if ((*numNeighbors)[partitionNode] == 0) continue;
    double imp = (*pagerankScore_)[partitionNode] / (*numNeighbors)[partitionNode];

    for (vector<long>::const_iterator partIt = (*outPartitions)[partitionNode].begin();
        partIt != (*outPartitions)[partitionNode].end(); ++partIt) {
      serializeImportance(*partIt, origNode, imp);
    }

    updateSelfScore(origNode, imp);
  }

  algo_->sendAndSignal(partIndex_);
}


void CleverPagerankNode::serializeImportance(int bufferIndex, long fromNode, double importance) {
  int shouldAdd = 1 + sizeof(long) + sizeof(double);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, fromNode);
  senderBuffer_->store(bufferIndex, importance);
}

void CleverPagerankNode::updateSelfScore(long origNode, double imp) {
  long start, end;
  if (inverseNodeBounds->find(origNode) != inverseNodeBounds->end()) {
    start = (*inverseNodeBounds)[origNode].first;
    end = (*inverseNodeBounds)[origNode].second;
    for (long outEdgeIt = start; outEdgeIt < end; ++outEdgeIt) {
      tmpScoreMutex_.lock();
      (*tmpScore_)[(*inverseOutEdges)[outEdgeIt]] += imp;
      tmpScoreMutex_.unlock();
    }
  }
}

void CleverPagerankNode::beforeIteration(string msg) {
  logger_->info("Starting iteration %d", actIter);
  for (long node = 0; node < (long) tmpScore_->size(); ++node) {
    (*tmpScore_)[node] = 0.0;
  }
}

bool CleverPagerankNode::afterIteration() {
  for (long node = 0; node < (long) tmpScore_->size(); ++node) {
    (*pagerankScore_)[node] = (*tmpScore_)[node] * (1.0 - dump_) + dump_ / allNode_;
    (*tmpScore_)[node] = 0.0;
  }
  return (++actIter < maxIter);
}

void CleverPagerankNode::final() {
  FILE* outf = fopen(outfile.c_str(), "w");
  if (NULL == outf) {
    logger_->error("Error opening file %s for writing.", outfile.c_str());
    return;
  }

  for (long node = 0; node < pagerankScore_->size(); ++node) {
    fprintf(outf, "%ld %.10lf\n", node + minNode_, (*pagerankScore_)[node]);
  }

  fclose(outf);
}

void CleverPagerankNode::setNumberNeighbors(vector<int>* nneighbors) {
  numNeighbors = nneighbors;
  pagerankScore_ = new vector<double>(nneighbors->size(), 0.0);
  tmpScore_ = new vector<double>(nneighbors->size(), 0.0);
}

void CleverPagerankNode::setOutPartitions(vector<vector<long> >* _outPartitions) {
  outPartitions = _outPartitions;
}

void CleverPagerankNode::setInverseNodeBounds(
    unordered_map<long, std::pair<long, long> >* _inverseNodeBounds) {
  inverseNodeBounds = _inverseNodeBounds;
}

void CleverPagerankNode::setInverseOutEdges(vector<long>* _inverseOutEdges) {
  inverseOutEdges = _inverseOutEdges;
}


