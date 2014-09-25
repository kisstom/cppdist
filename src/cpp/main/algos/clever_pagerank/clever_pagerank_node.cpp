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
  short size;

  for (long partitionNode = 0; partitionNode < (long) outPartitions->size(); ++partitionNode) {
    ++origNode;
    if ((*numNeighbors)[partitionNode] == 0) continue;
    double imp = (*pagerankScore_)[partitionNode] / (*numNeighbors)[partitionNode];

    size = (*outPartitions)[partitionNode][0];
    for (short c = 0; c < size; ++c) {
      if ((*outPartitions)[partitionNode][c + 1] == partIndex_) {
        updateSelfScore(origNode, imp);
      } else {
        serializeImportance((*outPartitions)[partitionNode][c + 1], origNode, imp);
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
      (*tmpScore_)[(*inverseOutEdges)[outEdgeIt] - minNode_] += imp;
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

void CleverPagerankNode::setOutPartitions(vector<short*>* _outPartitions) {
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
  logger_->info("Inverse node size: %d", inverseNodeBounds->size());

  inverseOutEdges = new vector<long>(upperBound);
  logger_->info("Resizing inverse out edges to %ld", upperBound);

  fclose(file);
}

void CleverPagerankNode::readInverseOutEdges(string fname) {
  FILE* file = fopen(fname.c_str(), "r");
  if (NULL == file) {
    logger_->info("Error opening inverse out edges: %s", fname.c_str());
    return;
  }

  logger_->info("Reading inverse out edges.");
  unsigned long numReadEdges = 0;
  long node;

  try {
    while (fscanf(file, "%ld\n", &node) != EOF) {
      (*inverseOutEdges)[numReadEdges] = node;
      ++numReadEdges;
      if (numReadEdges % 10000000 == 0) {
        logger_->info("%ld rows are read.", numReadEdges);
      }
    }
  }
  catch ( std::bad_alloc& ba) {
    logger_->info( "Bad allocation: %s", ba.what() );
  }
  catch ( std::exception &e) {
    logger_->info( "Unhandled exception: %s", e.what() );
  }
  catch ( ... ) {
    logger_->info( "Unknown exception" );
  }

  fclose(file);
  logger_->info("%ld edges stored", (long) inverseOutEdges->size());
}
