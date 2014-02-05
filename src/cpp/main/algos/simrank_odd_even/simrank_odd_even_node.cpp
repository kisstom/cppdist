/*
 * simrank_odd_even.cpp
 *
 *  Created on: 2014.01.13.
 *      Author: kisstom
 */

#include "simrank_odd_even_node.h"

SimrankOddEvenNode::SimrankOddEvenNode() {
	logger_ = &log4cpp::Category::getInstance(std::string("SimrankOddEvenNode"));
	fpIndex_ = 0;
	pathIndex_ = 0;
	pathLen_ = 0;
	numFingerprints_ = 0;
	oddIter_ = true;
}

SimrankOddEvenNode::SimrankOddEvenNode(short numFingerprints, short pathLen,
		int seed, GeneratorType genType, long num_nodes, long min_node, long nextMinNode) {
	logger_ = &log4cpp::Category::getInstance(std::string("SimrankOddEvenNode"));
	fpIndex_ = 0;
	pathIndex_ = 0;
	numFingerprints_ = numFingerprints;
	pathLen_ = pathLen;
	numNodes_ = num_nodes;
	minNode_ = min_node;
	nextMinNode_ = nextMinNode;
	initRandomGenerator(seed, genType);
	matrix_ = NULL;
	oddIter_ = true;
}

SimrankOddEvenNode::~SimrankOddEvenNode() {
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

void SimrankOddEvenNode::initRandomGenerator(int seed, GeneratorType type) {
	if (SRAND == type) {
	  random_ = new Random(seed);
	  logger_->info("RANDOM generator is set.");
	} else if (PSEUDO_RANDOM == type) {
		random_ = new HashPseudoRandom(seed);
		logger_->info("PSEUDO RANDOM generator is set.");
	} else {
		logger_->error("Unknown type of random generator %d", type);
	}
}

void SimrankOddEvenNode::beforeIteration() {
	if (oddIter_) {
	  logger_->info("\nBefore iteration.\nIter odd(paratlan) iter");
	} else {
	  logger_->info("\nBefore iteration.\nIter even(paros) iter");
	}
	logger_->info("Fingerprint %hd path %hd.", fpIndex_, pathIndex_);
}

bool SimrankOddEvenNode::incrementIndices() {
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

void SimrankOddEvenNode::incrementPathes() {
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


bool SimrankOddEvenNode::afterIteration() {
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

int SimrankOddEvenNode::hashToEdgeIndex(int hash, int length) {
	int index = hash % length;
	if (index < 0) {
		index += length;
	}

	return index;
}

long SimrankOddEvenNode::genEdge(long from) {
  int numNeighbors = matrix_->neighborhoodSize(from);
  if (numNeighbors <= 0) {
  	return -1;
  }

  int hash = random_->get(fpIndex_, pathIndex_, from);
  int index = hashToEdgeIndex(hash, numNeighbors);

  return matrix_->getEdgeAtPos(from, index);
}

void SimrankOddEvenNode::serializeRequest(long from, short bufferIndex) {
	int shouldAdd = 1 + sizeof(long);

	if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
		senderBuffer_->emptyBuffer(bufferIndex);
	}

	senderBuffer_->setBreak(bufferIndex);
	senderBuffer_->store(bufferIndex, from);
}

void SimrankOddEvenNode::serializeAnswer(long from, long to,
		short bufferIndex) {
	int shouldAdd = 1 + 2 * sizeof(long);

	if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
		senderBuffer_->emptyBuffer(bufferIndex);
	}

	senderBuffer_->setBreak(bufferIndex);
	senderBuffer_->store(bufferIndex, from);
	senderBuffer_->store(bufferIndex, to);
}

void SimrankOddEvenNode::storeRequest(long from, short partIndex) {
	sendBack_[from].push_back(partIndex);
}

void SimrankOddEvenNode::storeRequestedEdge(long from, long to) {
	nextNodesMutex_.lock();
  nextNodes_[from] = to;
  nextNodesMutex_.unlock();
}

void SimrankOddEvenNode::sender() {
	if (oddIter_) {
		senderOdd();
	} else {
		senderEven();
	}
}

void SimrankOddEvenNode::senderEven() {
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

void SimrankOddEvenNode::senderOdd() {
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

void SimrankOddEvenNode::initFromMaster(string) {
}

void SimrankOddEvenNode::initData(string partName) {
	logger_->info("Initing data.");
	matrix_ = new EdgelistContainer();
	matrix_->initContainers();
	// Igy egyszerubb beolvasni, de igazabol egy hack.
	matrix_->setMinnode(0);

	EdgeListBuilder builder;
	builder.setContainer(matrix_);
	builder.buildFromFile(partName);
	matrix_->setMinnode(minNode_);

	logger_->info("matrix data read");
	if (fpStartFname_.compare("NULL") == 0) {
		initStartForAll(minNode_, nextMinNode_, numNodes_, numFingerprints_);
	} else {
		FileUtil util(1024);
	  util.readFingerprintStart(minNode_, nextMinNode_, numFingerprints_, &fingerprints_,
			fpStartFname_, pathLen_ + 1);
	}
	finishedPathes_.resize(fingerprints_.size());
	logger_->info("fingerprint read");
}

void SimrankOddEvenNode::initStartForAll(long from, long to, long numnodes, short numFingerPrints) {
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

void SimrankOddEvenNode::final() {
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

void SimrankOddEvenNode::setMatrix(EdgelistContainer* matrix) {
	matrix_ = matrix;
}

void SimrankOddEvenNode::setFingerprints(vector<list<long*> > fingerprints) {
	fingerprints_ = fingerprints;
}

void SimrankOddEvenNode::setNextNodes(unordered_map<long, long> nextNodes) {
	nextNodes_ = nextNodes;
}

void SimrankOddEvenNode::initFinishedPathes(vector<vector<long*> > finishedPathes) {
	finishedPathes_ = finishedPathes;
}

void SimrankOddEvenNode::setOutputFile(string outputFile) {
	outFileName_ = outputFile;
}

void SimrankOddEvenNode::setFingerPrintFile(string fpStartFname) {
	fpStartFname_ = fpStartFname;
}

vector<list<long*> >* SimrankOddEvenNode::getPathes() {
	return &fingerprints_;
}

vector<vector<long*> >* SimrankOddEvenNode::getFinishedPathes() {
	return &finishedPathes_;
}

