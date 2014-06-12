/*
 * simrank_store_first_node.cpp
 *
 *  Created on: 2013.11.06.
 *      Author: kisstom
 */
#include "simrank_store_first_node.h"

SimrankStoreFirstNode::SimrankStoreFirstNode() {
	logger_ = &log4cpp::Category::getInstance(std::string("SimrankStoreFirstNode"));
	fpIndex_ = 0;
	pathIndex_ = 0;
	pathLen_ = 0;
	numFingerprints_ = 0;
}

SimrankStoreFirstNode::~SimrankStoreFirstNode() {
	for (int i = 0; i < (int) fingerprints_.size(); ++i) {
		//for (int j = 0; j < (int) fingerprints_[i].size(); ++j) {
		for (list<long*>::iterator it = fingerprints_[i].begin(); it != fingerprints_[i].end(); ++it) {
			delete[] *it;
		}
	}

	for (int i = 0; i < (int) finishedPathes_.size(); ++i) {
			//for (int j = 0; j < (int) fingerprints_[i].size(); ++j) {
			for (int j = 0; j < (int) finishedPathes_[i].size(); ++j) {
				delete[] finishedPathes_[i][j];
			}
		}

	if (matrix_) delete matrix_;
}

SimrankStoreFirstNode::SimrankStoreFirstNode(short numFingerprints, short pathLen,
		string fpStartFname, string outFileName) {
	fpIndex_ = 0;
	pathIndex_ = 0;
	numFingerprints_ = numFingerprints;
	pathLen_ = pathLen;
	fpStartFname_ = fpStartFname;
	outFileName_ = outFileName;
	generator_ = new RandomGenerator(13);
	//generator_->setSeed(13);
	matrix_ = NULL;
	logger_ = &log4cpp::Category::getInstance(std::string("SimrankStoreFirstNode"));
}

void SimrankStoreFirstNode::beforeIteration(string msg) {
	receiverFinished_ = 0;
	senderShouldStop_ = false;
}

bool SimrankStoreFirstNode::incrementIndices() {
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

bool SimrankStoreFirstNode::afterIteration() {
	logger_->info("After iteration started.");
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
	logger_->info("Finished pathes updated.");

	nextNodes_.clear();
	bool shouldCont = incrementIndices();
	return shouldCont;
}

long SimrankStoreFirstNode::genEdge(long from) {
  int numNeighbors = matrix_->neighborhoodSize(from);
  if (numNeighbors <= 0) {
  	return -1;
  }

  double uniR = generator_->uniRand(from);

  int index = (int) (numNeighbors * uniR);
  return matrix_->getEdgeAtPos(from, index);
}

void SimrankStoreFirstNode::serializeRequest(long from, short bufferIndex) {
	sendeBufferLock_.lock();
	int shouldAdd = 2 + sizeof(long);

	if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
		senderBuffer_->emptyBuffer(bufferIndex);
	}

	senderBuffer_->setBreak(bufferIndex);
	senderBuffer_->store(bufferIndex, '0');
	senderBuffer_->store(bufferIndex, from);
	sendeBufferLock_.unlock();
}

void SimrankStoreFirstNode::serializeAnswer(long from, long to,
		short bufferIndex) {
	sendeBufferLock_.lock();

	int shouldAdd = 2 + 2 * sizeof(long);

	if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
		senderBuffer_->emptyBuffer(bufferIndex);
	}

	senderBuffer_->setBreak(bufferIndex);
	senderBuffer_->store(bufferIndex, '1');
	senderBuffer_->store(bufferIndex, from);
	senderBuffer_->store(bufferIndex, to);
	sendeBufferLock_.unlock();
}

void SimrankStoreFirstNode::sender() {
	logger_->info("Sender started.");
	long pathEnd, nextEdge;
	short partIndex;
	long counter = 0;
	bool serialize = false;

  for (list<long*>::iterator it = fingerprints_[fpIndex_].begin();
  		it != fingerprints_[fpIndex_].end(); ++it) {
  	++counter;
  	//if (counter % 100000 == 0) {
    if (pathLen_ > 0 && counter % 100 == 0) {
  		logger_->info("%ld steps already done in sender", counter);
  	}

  	pathEnd = (*it)[pathIndex_];
  	partIndex = algo_->getPartitionIndex(pathEnd);

    if (partIndex == partIndex_) {
    	nextNodesMutex_.lock();
    	if (nextNodes_.find(pathEnd) == nextNodes_.end()) {
    	  nextEdge = genEdge(pathEnd);
    	  nextNodes_[pathEnd] = nextEdge;
    	}
    	nextNodesMutex_.unlock();
    } else {
    	nextNodesMutex_.lock();
  	  if (nextNodes_.find(pathEnd) == nextNodes_.end()) {
  	  	// IMPORTANT ezt be kell rakni mert kulonben tobbszor elkuldom
  		  nextNodes_[pathEnd] = -2;
  		  serialize = true;
  	  }
  	  nextNodesMutex_.unlock();
  	  if (serialize) {
  	  	serializeRequest(pathEnd, partIndex);
  	  	serialize = false;
  	  }
    }

  }

  senderFinishedDaemon();
  logger_->info("Sender finished.");
}

void SimrankStoreFirstNode::senderFinishedDaemon() {
	logger_->info("SenderFinishedDaemon started.");
  showSenderEndForAll();

  while (1) {
  	usleep(5000);
  	if (senderShouldStop_) {
  		logger_->info("Sender finished daemon should end.");
  		sendeBufferLock_.lock();
  		algo_->sendAndSignal(partIndex_);
  		sendeBufferLock_.unlock();
  		break;
  	}
  }
}

void SimrankStoreFirstNode::registerNodeFinished(short bufferI) {
	++receiverFinished_;

	if (receiverFinished_ == algo_->getNumberOfPartitions() - 1) {
		// TODO lock ?
		senderShouldStop_ = true;
	}
}

void SimrankStoreFirstNode::showSenderEndForAll() {
	for (short bufferI = 0; bufferI < algo_->getNumberOfPartitions(); ++bufferI) {
		if (bufferI == partIndex_) continue;
		showSenderEnd(bufferI);
	}
}

void SimrankStoreFirstNode::showSenderEnd(short bufferIndex) {
	sendeBufferLock_.lock();
	int shouldAdd = sizeof(char);

	if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
		senderBuffer_->emptyBuffer(bufferIndex);
	}

	senderBuffer_->setBreak(bufferIndex);
	// TODO use enum
	senderBuffer_->store(bufferIndex, '2');
	senderBuffer_->emptyBuffer(bufferIndex);
	//logger_->info("Sending end of sender to %hd", bufferIndex);
	sendeBufferLock_.unlock();
}

void SimrankStoreFirstNode::consumeRequest(long from, short bufferIndex) {
	nextNodesMutex_.lock();
	if (nextNodes_.find(from) == nextNodes_.end()) {
	  nextNodes_[from] = genEdge(from);
	}
	nextNodesMutex_.unlock();
	serializeAnswer(from, nextNodes_[from], bufferIndex);
}

void SimrankStoreFirstNode::consumeAnswer(long from, long to) {
	// TODO synch, store

	nextNodesMutex_.lock();
	nextNodes_[from] = to;
	nextNodesMutex_.unlock();
}


void SimrankStoreFirstNode::initFromMaster(string) {
}

void SimrankStoreFirstNode::block() {
	logger_->info("Running blocked.");
	while (1) {
		sleep(5000);
	}
}

void SimrankStoreFirstNode::initData(string partName, long minnode, long numnodes) {
	logger_->info("Initing data.");
	matrix_ = new EdgelistContainer();
	matrix_->initContainers();
	// Igy egyszerubb beolvasni.
	matrix_->setMinnode(0);

	EdgeListBuilder builder;
	builder.setContainer(matrix_);
	builder.buildFromFile(partName);
	matrix_->setMinnode(minnode);

	logger_->info("matrix data read");
	if (fpStartFname_.compare("NULL") == 0) {
		initStartForAll(algo_->getPartitionStartNode(partIndex_),
				algo_->getPartitionStartNode(partIndex_ + 1), numnodes, numFingerprints_);
	} else {
		FileUtil util(1024);
	  util.readFingerprintStart(algo_->getPartitionStartNode(partIndex_),
			algo_->getPartitionStartNode(partIndex_ + 1), numFingerprints_, &fingerprints_,
			fpStartFname_, pathLen_ + 1);
	}
	finishedPathes_.resize(fingerprints_.size());
	logger_->info("fingerprint read");
}

void SimrankStoreFirstNode::initStartForAll(long from, long to, long numnodes, short numFingerPrints) {
	fingerprints_.resize(numFingerPrints);

	long* path;
	short numElements = pathLen_ + 1;

	// not too good
	if (to == LONG_MAX) {
		to = from + numnodes;
	}

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

void SimrankStoreFirstNode::final() {
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
}

void SimrankStoreFirstNode::setFingerprints(vector<list<long*> > fingerprints) {
	fingerprints_ = fingerprints;
}

void SimrankStoreFirstNode::setNextNodes(unordered_map<long, long> nextNodes) {
	nextNodes_ = nextNodes;
}

void SimrankStoreFirstNode::initFinishedPathes(vector<vector<long*> > finishedPathes) {
	finishedPathes_ = finishedPathes;
}

vector<list<long*> >* SimrankStoreFirstNode::getPathes() {
	return &fingerprints_;
}

vector<vector<long*> >* SimrankStoreFirstNode::getFinishedPathes() {
	return &finishedPathes_;
}
