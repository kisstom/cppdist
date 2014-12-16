/*
 * custom_non_block_node.cpp
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#include "custom_multi_non_block_node.h"

CustomMultiNonBlockNode::CustomMultiNonBlockNode(long _allnode, long _minnode, double _dump, int _maxIter) {
   logger_ = &log4cpp::Category::getInstance(std::string("CustomMultiNonBlockNode"));

   allNode_ = _allnode;
   minNode_ = _minnode;
   dump_ = _dump;
   actIter = 0;
   maxIter = _maxIter;

   outPartitionHashes = NULL;
   inverseNodeBounds = NULL;
   inverseOutEdges = NULL;
   numneighbors = NULL;
   updateFlags = NULL;
   pagerankScore_ = NULL;
   tmpSenderScore_ = NULL;
   tmpReceiverScore_ = NULL;
   messageCounter = 0;
   numUpdates = 0;
 }


CustomMultiNonBlockNode::~CustomMultiNonBlockNode() {
  logger_->info("%d messages sent.", messageCounter);
  logger_->info("%d updates done.", numUpdates);
}

 void CustomMultiNonBlockNode::setOutputFileName(string _outfile) {
   outfile = _outfile;
 }

void CustomMultiNonBlockNode::sender() {
  logger_->info("Starting sender.");
  long origNode = minNode_ - 1;

  for (long partitionNode = 0; partitionNode < (long) outPartitionHashes->size(); ++partitionNode) {
    ++origNode;

    if ((*numneighbors)[partitionNode] == 0) continue;
    double imp = (*pagerankScore_)[partitionNode] / (*numneighbors)[partitionNode];

    if ((*outPartitionHashes)[partitionNode] >= 0) {
      serializeImportance((*outPartitionHashes)[partitionNode], origNode, imp);
    }

    if (updateFlags->at(partitionNode)) {
      updateSenderScore(origNode, imp);
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Finished sender.");
}


void CustomMultiNonBlockNode::serializeImportance(short hash, long fromNode,
    double importance) {
  //logger_->info("Starting serialize.");
  int shouldAdd = 1 + sizeof(long) + sizeof(double);

  if (!senderBuffer_->canAdd(hash, shouldAdd)) {
    senderBuffer_->emptyBuffer(hash);
  }

  //logger_->info("Serializing to %hd from %ld imp %.10lf.", hashIndex, fromNode, importance);
  senderBuffer_->setBreak(hash);
  senderBuffer_->store(hash, fromNode);
  senderBuffer_->store(hash, importance);
  ++messageCounter;
}

void CustomMultiNonBlockNode::updateReceiverScore(long origNode, double imp) {
  //logger_->info("Updating receiver score.");
  long start, end;
  if (inverseNodeBounds->find(origNode) != inverseNodeBounds->end()) {
    start = (*inverseNodeBounds)[origNode].first;
    end = (*inverseNodeBounds)[origNode].second;
    for (long outEdgeIt = start; outEdgeIt < end; ++outEdgeIt) {
      (*tmpReceiverScore_)[(*inverseOutEdges)[outEdgeIt] - minNode_] += imp;
      ++numUpdates;
    }
  }
}

void CustomMultiNonBlockNode::updateSenderScore(long origNode, double imp) {
  //logger_->info("Updating sender score.");
  long start, end;
  if (inverseNodeBounds->find(origNode) != inverseNodeBounds->end()) {
    //logger_->info("Looping inverseNodeBounds.");
    start = (*inverseNodeBounds)[origNode].first;
    end = (*inverseNodeBounds)[origNode].second;
    for (long outEdgeIt = start; outEdgeIt < end; ++outEdgeIt) {
      //logger_->info("Adding to edge %ld value %lf.", (*inverseOutEdges)[outEdgeIt] - minNode_, imp);
      (*tmpSenderScore_)[(*inverseOutEdges)[outEdgeIt] - minNode_] += imp;
    }
  }
}

void CustomMultiNonBlockNode::beforeIteration(string msg) {
  logger_->info("Starting iteration %d", actIter);
  for (long node = 0; node < (long) tmpSenderScore_->size(); ++node) {
    (*tmpSenderScore_)[node] = 0.0;
    (*tmpReceiverScore_)[node] = 0.0;
  }
}

bool CustomMultiNonBlockNode::afterIteration() {
  logger_->info("After iteration started.");
  for (long node = 0; node < (long) tmpSenderScore_->size(); ++node) {
    //logger_->info("%ld %lf %lf", minNode_ + node, (*tmpSenderScore_)[node], (*tmpReceiverScore_)[node]);
    (*pagerankScore_)[node] = ((*tmpSenderScore_)[node] + (*tmpReceiverScore_)[node]) *
        (1.0 - dump_) + dump_ / allNode_;
  }
  return (++actIter < maxIter);
}

void CustomMultiNonBlockNode::initFromMaster(string) {
  //multicastHelper = new MulticastHelper(algo_->getSlaveIndex());
}

void CustomMultiNonBlockNode::final() {
  FILE* outf = fopen(outfile.c_str(), "w");
  if (NULL == outf) {
    logger_->error("Error opening file %s for writing.", outfile.c_str());
    return;
  }

  for (long node = 0; node < (long) pagerankScore_->size(); ++node) {
    fprintf(outf, "%ld %.10lf\n", node + minNode_, (*pagerankScore_)[node]);
  }

  fclose(outf);
}

void CustomMultiNonBlockNode::setUpdateFlags(vector<bool>* _updateFlags) {
  updateFlags = _updateFlags;
  double ini = 1.0 / allNode_;
  pagerankScore_ = new vector<double>(updateFlags->size(), ini);
  tmpSenderScore_ = new vector<double>(updateFlags->size(), 0.0);
  tmpReceiverScore_ = new vector<double>(updateFlags->size(), 0.0);
}

void CustomMultiNonBlockNode::setOutpartitionHashes(vector<short>* _outPartitionHashes) {
  outPartitionHashes = _outPartitionHashes;
}

void CustomMultiNonBlockNode::setNumneighbors(vector<int>* _numneighbors) {
  numneighbors = _numneighbors;
}

void CustomMultiNonBlockNode::setInverseNodeBounds(
    unordered_map<long, std::pair<long, long> >* _inverseNodeBounds) {
  inverseNodeBounds = _inverseNodeBounds;
}

void CustomMultiNonBlockNode::setInverseOutEdges(vector<long>* _inverseOutEdges) {
  inverseOutEdges = _inverseOutEdges;
}

void CustomMultiNonBlockNode::readInverseNodeBounds(string fname) {
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

void CustomMultiNonBlockNode::readInverseOutEdges(string fname) {
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



