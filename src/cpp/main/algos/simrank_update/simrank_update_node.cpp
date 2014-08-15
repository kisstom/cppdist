/*
 * node.cpp

 *
 *  Created on: 2013.07.30.
 *      Author: kisstom
 */

#include "simrank_update_node.h"

SimrankUpdateNode::SimrankUpdateNode() {
  fpIndex_ = 0;
  edgeInPath_ = 0;
  fpReader_ = NULL;
  setSeed(13);
}

SimrankUpdateNode::SimrankUpdateNode(char* fpFile, char* slaveryConfigFile, int numPathes, FILE* output_file) {
	strcpy(fpFileName_, fpFile);
	strcpy(slaveryConfigFileName_, slaveryConfigFile);
	fpIndex_ = 0;
	edgeInPath_ = 0;
	numPathes_ = numPathes;
	output_file_ = output_file;
	fpReader_ = NULL;
	setSeed(13);
	logger_ = &log4cpp::Category::getInstance(std::string("SimrankUpdateNode"));
}

SimrankUpdateNode::~SimrankUpdateNode() {
	//delete matrix_;
	if (NULL != fpReader_) delete fpReader_;
}

long SimrankUpdateNode::genEdge(RandomWalk* rw) {
	logger_->info("gen edge, enum %hd einpath %hd from %ld fingerprint index %hd", rw->edge_num, edgeInPath_, rw->buf[edgeInPath_], fpIndex_);
	genEdgethreadLock_.lock();
	if (nextEdgeMap_.find(rw->buf[edgeInPath_]) == nextEdgeMap_.end()) {
		nextEdge(rw);
		nextEdgeMap_[rw->buf[edgeInPath_]] = rw->buf[edgeInPath_ + 1];
	}
	genEdgethreadLock_.unlock();
	return nextEdgeMap_[rw->buf[edgeInPath_]];
}

void SimrankUpdateNode::printFp(RandomWalk* rw) {
	for (int i = 0; i <= rw->edge_num; ++i) {
		logger_->info("%ld", rw->buf[i]);
	}
}

long SimrankUpdateNode::nextEdge(RandomWalk* rw) {
	long edge = -1;
	if (rw->edge_num <= edgeInPath_) {
		edge = generateRandomNeighbour(rw->buf[edgeInPath_], matrix_->getUnchangedMatrix(),
				matrix_->getNewlyAddedMatrix());

	} else {
		edge = changeOrPreserve(rw->buf[edgeInPath_], rw->buf[edgeInPath_ + 1]);
	}

	incrementRandomWalkSize(rw, edge);

	return edge;
}

void SimrankUpdateNode::incrementRandomWalkSize(RandomWalk* rw, long edge) {
	// Nincs kiszomszed, vagy marad az eredeti el.
	// Ekkor nem kell az edge_num-mal foglalkozni.
	// Ha nincs kiszomszed es vege a setanak, akkor az edge_num-ot
	// at kell allitani, ezt lehet hogy itt kellene(TODO).
	if (edge < 0) return;

	// Uj el, noveljuk az elszamot.
	if (rw->edge_num <= edgeInPath_) {
		rw->edge_num++;
	} else {
		// Generaltunk uj elt a regi helyett, innentol ezt az uj setat folytatjuk.
		rw->edge_num = edgeInPath_ + 1;
	}
	// Eltaroljuk az uj elt.
	rw->buf[edgeInPath_ + 1] = edge;
}

void SimrankUpdateNode::sender() {
	logger_->info("Starting sender.");
	long edge;
	list<RandomWalk>::iterator it = currentFp_->begin();
	int n = 0;
  while (it != currentFp_->end()) {
  	//logger_->info("currentFp_ size %d", n++);
    if (fpIndex_ == it->fp_index) {
      int partIndex = algo_->getPartitionIndex(it->buf[edgeInPath_]);
      if (partIndex == partIndex_) {
      	//logger_->info("Part index %d for node %ld", partIndex_, it->buf[edgeInPath_]);
        edge = nextEdge(&(*it));
        //edge = genEdge(&(*it));
        if (edge != -1) {
        	if (edgeInPath_ >= it->edge_num) {
        		logger_->warn("Edge update problem here: edge num %hd index %hd rnd edge %ld", it->edge_num, edgeInPath_, edge);
        		printFp(&(*it));
        	}
        	++it;
        } else {
					// finishes the path

        	finishPath(&(*it));
					it = currentFp_->erase(it);
				}

				continue;
      }

      serializeToSender(*it, partIndex);
      it = currentFp_->erase(it);
    } else {
    	++it;
    }
  }

  algo_->sendAndSignal(partIndex_);
  logger_->info("Sender finished.");
}

void SimrankUpdateNode::finishPath(RandomWalk* rw) {
	rw->edge_num = edgeInPath_;
	threadLock_.lock();
	finishedFp_.push_back(*rw);
	threadLock_.unlock();
}

void SimrankUpdateNode::beforeIteration(string msg) {
	/*logger_->info("Current fp size: %d", currentFp_->size());
	logger_->info("Finished fp size: %d", finishedFp_.size());*/
	logger_->info("Before iteration. Fp index %d edge in path %d", fpIndex_, edgeInPath_);
	//rand_outlinks_.clear();
}

bool SimrankUpdateNode::afterIteration() {
	logger_->info("After iteration.");
	nextEdgeMap_.clear();
	for (int i = 0; i < (int) tmpFp_.size(); ++i) {
		currentFp_->push_back(tmpFp_[i]);
	}
  tmpFp_.clear();

	if (edgeInPath_ == 10) return false;

	++fpIndex_;
	if (fpIndex_ == numPathes_) {
		fpIndex_ = 0;
		++edgeInPath_;
		//logger_->info("After iteration edge in path %hd", edgeInPath_);
		if (edgeInPath_ == 10) {
			return false;
		}
	}
	return true;
}

void SimrankUpdateNode::initData(string path, long minnode, long numNodes) {
	matrix_ = new SortedVectorNodeMatrix();
	fpReader_ = new FingerprintReader();
	currentFp_ = new list<RandomWalk>();
	fpReader_->setRWContainer(currentFp_);

	PartitionInfo info;
	info.fname = path;
	info.minnode = minnode;

	logger_->info("Min node %d.", minnode);
	// TODO ezt valami inicializalos reszbe
	// matrix_->setMinnode(minnode);
	matrix_->setNumnodes(numNodes);
	matrix_->storeMatrixes(info, slaveryConfigFileName_);

	//char prefix[1024];
	//sprintf(prefix, "/home/kisstom/tmpPartitions/part_%d_", partIndex_);
	//matrix_->flushAll(string(prefix));
	fpReader_->readFile(fpFileName_, algo_->getPartitionStartNode(partIndex_),
			algo_->getPartitionStartNode(partIndex_ + 1));
}

void SimrankUpdateNode::initFromMaster(string ss) {
}

void SimrankUpdateNode::update(RandomWalk* rw) {
	long edge = nextEdge(rw);
	//long edge = genEdge(rw);
	if (edge == -1) {
		finishPath(rw);
	} else {
		if (edgeInPath_ >= rw->edge_num) {
		  logger_->warn("Edge update problem after receive: edge num %d index %d rnd edge %ld", rw->edge_num, edgeInPath_, edge);
		  printFp(rw);
		}
		tmpFp_.push_back(*rw);
	}
}

void SimrankUpdateNode::setSeed(int s) {
	//randomGenerator_.setSeed(s);
	randomGenerator_ = new RandomGenerator(s);
}

void SimrankUpdateNode::setMatrix(SortedVectorNodeMatrix* matrix) {
	matrix_ = matrix;
}

long SimrankUpdateNode::generateRandomNeighbour(long node, EdgelistContainer* container) {
	int neighborSize = container->neighborhoodSize(node);
	if (neighborSize <= 0) return -1;

	double uniR = randomGenerator_->uniRand();

	int index = (int) (neighborSize * uniR);
	return container->getEdgeAtPos(node, index);
}

long SimrankUpdateNode::generateRandomNeighbour(long node, EdgelistContainer* cont1,
		EdgelistContainer* cont2) {
	int neighborSize1 = cont1->neighborhoodSize(node);
	int neighborSize2 = cont2->neighborhoodSize(node);

	if (neighborSize1 < 0 && neighborSize2 < 0) {
		logger_->warn("Node %ld has edge stored nor in old neither in new data", node);
		return -1;
	}
	if (neighborSize1 < 0) neighborSize1 = 0;
	if (neighborSize2 < 0) neighborSize2 = 0;

	double uniR = randomGenerator_->uniRand();

	int index = (int) ((neighborSize1 + neighborSize2) * uniR);
	if (index < neighborSize1) {
		return cont1->getEdgeAtPos(node, index);
	}

	return cont2->getEdgeAtPos(node, index - neighborSize1);
}

bool SimrankUpdateNode::flipCoinToStay(long node) {
	int unchanged = matrix_->getUnchangedMatrix()->neighborhoodSize(node);
	int updated = matrix_->getNewlyAddedMatrix()->neighborhoodSize(node);

	if (updated < 0 || unchanged < 0) return true;
	if (updated + unchanged == 0) return true;

	double uniR = randomGenerator_->uniRand();
	if (uniR < (double) unchanged / (updated + unchanged)) return true;
	return false;
}

long SimrankUpdateNode::changeOrPreserve(long from, long to) {
	bool isDeleted = matrix_->isEdgeDeleted(from, to);

	if (isDeleted) {
    return generateRandomNeighbour(from, matrix_->getUnchangedMatrix(),
    		matrix_->getNewlyAddedMatrix());
	}

	bool stay = flipCoinToStay(from);
	if (stay) return -2;

	return generateRandomNeighbour(from, matrix_->getNewlyAddedMatrix());
}

void SimrankUpdateNode::serializeToSender(const RandomWalk& rw, int bufferIndex) {
	int shouldAdd = sizeof(short) * 2 + sizeof(long) * (rw.edge_num + 1) + 1;
	if (!senderBuffer_->canAdd(bufferIndex, shouldAdd)) {
		senderBuffer_->emptyBuffer(bufferIndex);
	}

	senderBuffer_->setBreak(bufferIndex);
  senderBuffer_->store(bufferIndex, rw.edge_num);
  for (int i = 0; i <= rw.edge_num; ++i) {
  	senderBuffer_->store(bufferIndex, rw.buf[i]);
  }
  senderBuffer_->store(bufferIndex, rw.fp_index);
  //senderBuffer_->setBreak(bufferIndex);
}

void SimrankUpdateNode::final() {
	logger_->info("finished size %d", finishedFp_.size());
	for (int i = 0; i < (int) finishedFp_.size(); ++i) {
		fprintf(output_file_, "%hd ", finishedFp_[i].fp_index);
		for (int j = 0; j < finishedFp_[i].edge_num; ++j) {
			fprintf(output_file_, "%ld ", finishedFp_[i].buf[j]);
		}
		fprintf(output_file_, "%ld\n", finishedFp_[i].buf[finishedFp_[i].edge_num]);
	}

	logger_->info("current size %d", currentFp_->size());
	list<RandomWalk>::iterator it = currentFp_->begin();

	while (it!= currentFp_->end()) {
		fprintf(output_file_, "%hd ", it->fp_index);
		for (int j = 0; j < it->edge_num; ++j) {
			fprintf(output_file_, "%ld ", it->buf[j]);
		}
		fprintf(output_file_, "%ld\n", it->buf[it->edge_num]);
		++it;
	}

	logger_->info("Simrank update node finished.");
}

void SimrankUpdateNode::setFpIndex(short fpIndex) {
	fpIndex_ = fpIndex;
}

void SimrankUpdateNode::setEdgeInPathes(short edgeInPath) {
	edgeInPath_ = edgeInPath;
}
