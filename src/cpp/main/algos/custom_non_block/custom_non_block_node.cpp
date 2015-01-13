/*
 * custom_non_block_node.cpp
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#include "custom_non_block_node.h"

CustomNonBlockNode::CustomNonBlockNode(long _allnode, long _minnode, double _dump, int _maxIter) {
   logger_ = &log4cpp::Category::getInstance(std::string("CustomNonBlockNode"));

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
   tmpSenderScore_ = NULL;
   tmpReceiverScore_ = NULL;
   partConfHandler = NULL;
 }

void CustomNonBlockNode::setPartitionConfigHandler(GraphPartitionConfigHandler* configHandler) {
  partConfHandler = configHandler;
}

 void CustomNonBlockNode::setOutputFileName(string _outfile) {
   outfile = _outfile;
 }

void CustomNonBlockNode::sender() {
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
        updateSenderScore(origNode, imp);
      } else {
        serializeImportance((*outPartitions)[partitionNode][c + 1], origNode, imp);
      }
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}


void CustomNonBlockNode::serializeImportance(short bufferIndex, long fromNode, double importance) {
  int shouldAdd = 1 + sizeof(long) + sizeof(double);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, fromNode);
  senderBuffer_->store(bufferIndex, importance);
}

void CustomNonBlockNode::updateReceiverScore(long origNode, double imp) {
  long start, end;
  if (inverseNodeBounds->find(origNode) != inverseNodeBounds->end()) {
    start = (*inverseNodeBounds)[origNode].first;
    end = (*inverseNodeBounds)[origNode].second;
    for (long outEdgeIt = start; outEdgeIt < end; ++outEdgeIt) {
      (*tmpReceiverScore_)[(*inverseOutEdges)[outEdgeIt] - minNode_] += imp;
    }
  }
}

void CustomNonBlockNode::updateSenderScore(long origNode, double imp) {
  long start, end;
  if (inverseNodeBounds->find(origNode) != inverseNodeBounds->end()) {
    start = (*inverseNodeBounds)[origNode].first;
    end = (*inverseNodeBounds)[origNode].second;
    for (long outEdgeIt = start; outEdgeIt < end; ++outEdgeIt) {
      (*tmpSenderScore_)[(*inverseOutEdges)[outEdgeIt] - minNode_] += imp;
    }
  }
}

void CustomNonBlockNode::beforeIteration(string msg) {
  logger_->info("Starting iteration %d", actIter);
  for (long node = 0; node < (long) tmpSenderScore_->size(); ++node) {
    (*tmpSenderScore_)[node] = 0.0;
    (*tmpReceiverScore_)[node] = 0.0;
  }
}

bool CustomNonBlockNode::afterIteration() {
  logger_->info("After iteration started.");
  for (long node = 0; node < (long) tmpSenderScore_->size(); ++node) {
    (*pagerankScore_)[node] = ((*tmpSenderScore_)[node] + (*tmpReceiverScore_)[node]) *
        (1.0 - dump_) + dump_ / allNode_;
  }
  return (++actIter < maxIter);
}

void CustomNonBlockNode::final() {
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

void CustomNonBlockNode::setNumberNeighbors(vector<int>* nneighbors) {
  numNeighbors = nneighbors;
  double ini = 1.0 / allNode_;
  pagerankScore_ = new vector<double>(nneighbors->size(), ini);
  tmpSenderScore_ = new vector<double>(nneighbors->size(), 0.0);
  tmpReceiverScore_ = new vector<double>(nneighbors->size(), 0.0);
}

void CustomNonBlockNode::setOutPartitions(vector<short*>* _outPartitions) {
  outPartitions = _outPartitions;
}

void CustomNonBlockNode::setInverseNodeBounds(
    unordered_map<long, std::pair<long, long> >* _inverseNodeBounds) {
  inverseNodeBounds = _inverseNodeBounds;
}

void CustomNonBlockNode::setInverseOutEdges(vector<long>* _inverseOutEdges) {
  inverseOutEdges = _inverseOutEdges;
}

void CustomNonBlockNode::readInverseNodeBounds(string fname) {
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

void CustomNonBlockNode::readInverseOutEdges(string fname) {
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



