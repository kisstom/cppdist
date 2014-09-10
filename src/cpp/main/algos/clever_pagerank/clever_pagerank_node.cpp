/*
 * clever_pagerank.cpp
 *
 *  Created on: 2014.09.09.
 *      Author: kisstom
 */

#include "clever_pagerank_node.h"

CleverPagerankNode::CleverPagerankNode(long _allnode, long _minnode, double _dump, int _maxIter) {
   logger_ = &log4cpp::Category::getInstance(std::string("CleverPagerankNode"));

   allNode_ = _allnode;
   minNode_ = _minnode;
   dump_ = _dump;
   actIter = 0;
   maxIter = _maxIter;

   outPartitions = NULL;
   inverseNodeBounds = NULL;
   inverseOutEdges = NULL;
   numNeighbors = NULL;
   pagerankScore_ = NULL;
   tmpScore_ = NULL;
 }

 void CleverPagerankNode::setOutputFileName(string _outfile) {
   outfile = _outfile;
 }

void CleverPagerankNode::sender() {
  logger_->info("Starting sender.");
  long origNode = minNode_ - 1, start, end;

  for (long partitionNode = 0; partitionNode < (long) outPartitions->size(); ++partitionNode) {
    ++origNode;
    if ((*numNeighbors)[partitionNode] == 0) continue;
    double imp = (*pagerankScore_)[partitionNode] / (*numNeighbors)[partitionNode];

    for (set<int>::const_iterator partIt = (*outPartitions)[partitionNode].begin();
        partIt != (*outPartitions)[partitionNode].end(); ++partIt) {
      if (*partIt == partIndex_) {
        updateSelfScore(origNode, imp);
      } else {
        serializeImportance(*partIt, origNode, imp);
      }
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}


void CleverPagerankNode::serializeImportance(int bufferIndex, long fromNode, double importance) {
  int shouldAdd = 1 + sizeof(long) + sizeof(double);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  logger_->info("Serializing: %ld %lf", fromNode, importance);
  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, fromNode);
  senderBuffer_->store(bufferIndex, importance);
}

void CleverPagerankNode::updateSelfScore(long origNode, double imp) {
  logger_->info("Updating self score.");
  long start, end;
  if (inverseNodeBounds->find(origNode) != inverseNodeBounds->end()) {
    start = (*inverseNodeBounds)[origNode].first;
    end = (*inverseNodeBounds)[origNode].second;
    for (long outEdgeIt = start; outEdgeIt < end; ++outEdgeIt) {
      tmpScoreMutex_.lock();
      (*tmpScore_)[(*inverseOutEdges)[outEdgeIt] - minNode_] += imp;
      tmpScoreMutex_.unlock();
    }
  }
  logger_->info("Self score updated.");
}

void CleverPagerankNode::beforeIteration(string msg) {
  logger_->info("Starting iteration %d", actIter);
  for (long node = 0; node < (long) tmpScore_->size(); ++node) {
    (*tmpScore_)[node] = 0.0;
  }
}

bool CleverPagerankNode::afterIteration() {
  logger_->info("After iteration started.");
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

void CleverPagerankNode::setOutPartitions(vector<set<int> >* _outPartitions) {
  outPartitions = _outPartitions;
}

void CleverPagerankNode::setInverseNodeBounds(
    unordered_map<long, std::pair<long, long> >* _inverseNodeBounds) {
  inverseNodeBounds = _inverseNodeBounds;
}

void CleverPagerankNode::setInverseOutEdges(vector<long>* _inverseOutEdges) {
  inverseOutEdges = _inverseOutEdges;
}

void CleverPagerankNode::readInverseNodeBounds(string fname) {
  FILE* file = fopen(fname.c_str(), "r");
  if (NULL == file) {
    logger_->info("Error opening inverse node bounds: %s", fname.c_str());
    return;
  }


  long prevBound = 0, node, upperBound;
  inverseNodeBounds = new unordered_map<long, std::pair<long, long> >();

  while (fscanf(file, "%ld %ld\n", &node, &upperBound) != EOF) {
    (*inverseNodeBounds)[node] = std::make_pair<long, long>(prevBound, upperBound);
    prevBound = upperBound;
  }

  fclose(file);
}

void CleverPagerankNode::readInverseOutEdges(string fname) {
  FILE* file = fopen(fname.c_str(), "r");
  if (NULL == file) {
    logger_->info("Error opening inverse out edges: %s", fname.c_str());
    return;
  }
  long node;

  logger_->info("Reading inverse out edges.");
  inverseOutEdges = new vector<long>();
  while (fscanf(file, "%ld\n", &node) != EOF) {
    logger_->info("inverseout edge %ld", node);
    inverseOutEdges->push_back(node);
  }

  fclose(file);
  logger_->info("%d edges stored", inverseOutEdges->size());
}
