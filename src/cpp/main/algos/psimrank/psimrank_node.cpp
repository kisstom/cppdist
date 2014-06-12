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
    int seed, long num_nodes, long min_node, long nextMinNode) {
  logger_ = &log4cpp::Category::getInstance(std::string("PSimrankNode"));
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
    sscanf(msg.c_str(), "%ld %ld", &aCoef_, &bCoef_);
    logger_->info("New coeffs %s %ld %ld", aCoef_, bCoef_);
  } else {
    logger_->info("\nBefore iteration.\nIter even(paros) iter");
  }
  logger_->info("Fingerprint %hd path %hd.", fpIndex_, pathIndex_);
}

bool PSimrankNode::incrementIndices() {
  if (fpIndex_ + 1 < numFingerprints_) {
    ++fpIndex_;
  } else {
    if (pathIndex_ + 1 < pathLen_) {
      fpIndex_ = 0;
      ++pathIndex_;
      logger_->info("Incrementing path index %hd", pathIndex_);
    } else {
      return false;
    }
  }

  return true;
}


void PSimrankNode::incrementPathes() {
  list<long*>::iterator it = fingerprints_[fpIndex_].begin();
  while (it != fingerprints_[fpIndex_].end()) {
    (*it)[pathIndex_ + 1] = nextNodes_[(*it)[pathIndex_]];
    if ((*it)[pathIndex_ + 1] < 0) {
      finishedPathes_[fpIndex_].push_back(*it);
      it = fingerprints_[fpIndex_].erase(it);
    } else {
      ++it;
    }
  }
}

bool PSimrankNode::afterIteration() {
  logger_->info("After iteration started.");
  if (oddIter_) {
    oddIter_ = false;
    return true;
  }

  incrementPathes();
  logger_->info("Finished pathes updated.");

  nextNodes_.clear();
  sendBack_.clear();
  oddIter_ = true;

  bool shouldCont = incrementIndices();
  return shouldCont;
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


void PSimrankNode::serializeRequest(long from, short bufferIndex) {
  int shouldAdd = 1 + sizeof(long);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, from);
}

void PSimrankNode::serializeAnswer(long from, long to,
    short bufferIndex) {
  int shouldAdd = 1 + 2 * sizeof(long);

  if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
    senderBuffer_->emptyBuffer(bufferIndex);
  }

  senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, from);
  senderBuffer_->store(bufferIndex, to);
}

void PSimrankNode::storeRequest(long from, short partIndex) {
  sendBack_[from].push_back(partIndex);
}

void PSimrankNode::storeRequestedEdge(long from, long to) {
  nextNodesMutex_.lock();
  nextNodes_[from] = to;
  nextNodesMutex_.unlock();
}

void PSimrankNode::sender() {
  if (oddIter_) {
    senderOdd();
  } else {
    senderEven();
  }
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

void PSimrankNode::initFromMaster(string) {
}

void PSimrankNode::initEdgeListContainer(string partName) {
  logger_->info("Initing edge list container.");

  matrix_ = new EdgelistContainer();
  matrix_->initContainers();
  matrix_->setMinnode(minNode_);

  edgeListbuilder_->setContainer(matrix_);
  edgeListbuilder_->buildFromFile(partName);

  logger_->info("Edge list container inited.");
}

void PSimrankNode::initFingerprints() {
  logger_->info("Starting fingerprint read.");
  if (fpStartFname_.compare("NULL") == 0) {
    initStartForAll(minNode_, nextMinNode_, numNodes_, numFingerprints_);
  } else {
    FileUtil util(1024);
    util.readFingerprintStart(minNode_, nextMinNode_, numFingerprints_, &fingerprints_,
      fpStartFname_, pathLen_ + 1);
  }
  finishedPathes_.resize(fingerprints_.size());
  logger_->info("Fingerprints read.");
}

void PSimrankNode::initData(string partName) {
  initEdgeListContainer(partName);
  initFingerprints();
}

void PSimrankNode::initStartForAll(long from, long to, long numnodes, short numFingerPrints) {
  fingerprints_.resize(numFingerPrints);

  long* path;
  short numElements = pathLen_ + 1;

  logger_->info("Initing %hd fingerprints from %ld to %ld", numFingerPrints, from, to);
  for (long node = from; node < to; ++node) {
    for (int fpIndex = 0; fpIndex < numFingerPrints; ++fpIndex) {
      path = new long[numElements];
      std::fill_n(path, numElements, -1);
      path[0] = node;
      path[1] = node;
      fingerprints_[fpIndex].push_back(path);
    }
  }
  logger_->info("Fingerprints init finished.");
}

void PSimrankNode::final() {
  logger_->info("Final.");
  FILE* outputFile = fopen(outFileName_.c_str(), "w");
  if (NULL == outputFile) {
    logger_->error("Error opening fingerprint output file %s", outFileName_.c_str());
    return;
  }
  int pathIndex;

  for (int fpIndex = 0; fpIndex < (int) fingerprints_.size(); ++fpIndex) {
    for (list<long*>::iterator it = fingerprints_[fpIndex].begin(); it != fingerprints_[fpIndex].end(); ++it) {
      fprintf(outputFile, "%d", fpIndex);
      pathIndex = 0;
      while (pathIndex <= pathLen_ && (*it)[pathIndex] != -1 ) {
        fprintf(outputFile, " %ld", (*it)[pathIndex]);
        ++pathIndex;
      }
      fprintf(outputFile, "\n");
    }

    for (vector<long*>::iterator it = finishedPathes_[fpIndex].begin(); it != finishedPathes_[fpIndex].end(); ++it) {
      fprintf(outputFile, "%d", fpIndex);
      pathIndex = 0;
      while (pathIndex <= pathLen_ && (*it)[pathIndex] != -1) {
        fprintf(outputFile, " %ld", (*it)[pathIndex]);
        ++pathIndex;
      }
      fprintf(outputFile, "\n");
    }
  }

  fclose(outputFile);
  logger_->info("Fingerprints writed to output file.");
}

void PSimrankNode::setMatrix(EdgelistContainer* matrix) {
  matrix_ = matrix;
}

void PSimrankNode::setFingerprints(vector<list<long*> > fingerprints) {
  fingerprints_ = fingerprints;
  finishedPathes_.resize(fingerprints.size());
}

void PSimrankNode::setNextNodes(unordered_map<long, long> nextNodes) {
  nextNodes_ = nextNodes;
}

void PSimrankNode::initFinishedPathes(vector<vector<long*> > finishedPathes) {
  finishedPathes_ = finishedPathes;
}

void PSimrankNode::setOutputFile(string outputFile) {
  outFileName_ = outputFile;
}

void PSimrankNode::setFingerPrintFile(string fpStartFname) {
  fpStartFname_ = fpStartFname;
}

vector<list<long*> >* PSimrankNode::getPathes() {
  return &fingerprints_;
}

vector<vector<long*> >* PSimrankNode::getFinishedPathes() {
  return &finishedPathes_;
}

void PSimrankNode::setEdgeListBuilder(IEdgeListBuilder* builder) {
  edgeListbuilder_ = builder;
}
