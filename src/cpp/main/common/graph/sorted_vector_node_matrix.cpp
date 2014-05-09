/*
 * sorted_vector_node_matrix.cpp

 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */


#include "sorted_vector_node_matrix.h"


SortedVectorNodeMatrix::SortedVectorNodeMatrix() :
MAX_ROW_LENGTH_(15000000) {
	shouldDeleteEdgelistContainers_ = false;
  logger_ = &log4cpp::Category::getInstance(std::string("SortedVectorNodeMatrix"));
}

void SortedVectorNodeMatrix::setMinnode(long minnode) {
	minnode_ = minnode;
}

void SortedVectorNodeMatrix::setNumnodes(long numnodes) {
	logger_->info("Setting num nodes %ld", numnodes);
	numnodes_ = numnodes;
}

bool SortedVectorNodeMatrix::shouldDeleteEdgelistContainers() {
	return shouldDeleteEdgelistContainers_;
}

EdgelistContainer* SortedVectorNodeMatrix::getUnchangedMatrix() {
	return unchangedMatrix_;
}

EdgelistContainer* SortedVectorNodeMatrix::getNewlyAddedMatrix() {
	return newlyAddedMatrix_;
}

EdgelistContainer* SortedVectorNodeMatrix::getDeletedMatrix() {
	return deletedMatrix_;
}

PartitionInfo SortedVectorNodeMatrix::checkThisPartition(char* line) {
	long minnode, numnodes;
	char fname[1024];
	PartitionInfo pinfo;


	sscanf(line, "%*d %s %ld %*d %ld", fname, &numnodes, &minnode);
	pinfo.minnode = minnode;
	if ((minnode_ >= minnode && minnode + numnodes > minnode_) ||
			(minnode >= minnode_ && minnode < minnode_ + numnodes_)) {
		pinfo.fname = string(fname);
		return pinfo;
	}
	pinfo.fname = string("");
	return pinfo;
}

vector<PartitionInfo> SortedVectorNodeMatrix::getOldPartitionNames(char* slavery_path) {
	logger_->info("Getting old partition names.");
	FILE* slavery_file = fopen(slavery_path, "r");
	if (NULL == slavery_file) {
		logger_->info("ERROR opening slavery file %s\n", slavery_path);
	}

	char line[1024];
	PartitionInfo part_name;
	vector<PartitionInfo> retval;
	while (NULL != fgets(line, 1024, slavery_file)) {
		part_name = checkThisPartition(line);
		if (part_name.fname.compare("") != 0) {
			retval.push_back(part_name);
		}
	}

	fclose(slavery_file);
	logger_->info("Old partition names read.");
	return retval;
}

void SortedVectorNodeMatrix::storeMatrixes(PartitionInfo info, char* oldSlaveryFile) {
	logger_->info("Storing matrices.");
  initEdgelistContainers();
  initEdgelists();
  newlyAddedMatrix_->setMinnode(info.minnode);
  deletedMatrix_->setMinnode(info.minnode);
  unchangedMatrix_->setMinnode(info.minnode);
  this->setMinnode(info.minnode);

  vector<PartitionInfo> neededOldPartitions = getOldPartitionNames(oldSlaveryFile);
  if (neededOldPartitions.size() == 0) {
  	readUpdatedMatrix(info.fname);
  } else {
    readUpdatedMatrix(info.fname, neededOldPartitions);
  }
}

SortedVectorNodeMatrix::~SortedVectorNodeMatrix() {
  if (shouldDeleteEdgelistContainers_) {
  	delete newlyAddedMatrix_;
  	delete deletedMatrix_;
  	delete unchangedMatrix_;
  }
}

void SortedVectorNodeMatrix::readUpdatedMatrix(string updatedMatrixFName) {
	logger_->info("Reading updated matrix.");
	FILE* updatedMatrix = fopen(updatedMatrixFName.c_str(), "r");
	if (NULL == updatedMatrix) {
		printf("ERROR opening file %s.\n", updatedMatrixFName.c_str());
	  return;
	}
	char* line = new char[MAX_ROW_LENGTH_];
	vector<long> edges;
	long nodeId = minnode_;
	char* success;
	while (true) {
		success = fgets(line, MAX_ROW_LENGTH_, updatedMatrix);
		if (NULL == success) break;
		Util::readEdges(line, &edges);
		storeInUpdatedMatrix(edges.begin(), edges.end(), edges.end(), edges.end(), nodeId);
		edges.clear();
		++nodeId;
	}

	finishMatrix();
	fclose(updatedMatrix);
	delete[] line;
	logger_->info("Reading finished.");
}

void SortedVectorNodeMatrix::readUpdatedMatrix(string updatedMatrixFName,
		vector<PartitionInfo> oldPartitionNames) {
	logger_->info("Reading updated matrix.");
	FILE* updatedMatrix = fopen(updatedMatrixFName.c_str(), "r");
	if (NULL == updatedMatrix) {
		printf("ERROR opening file %s.\n", updatedMatrixFName.c_str());
    return;
	}

	FILE* oldMatrixPartition = NULL;
  char* line1 = new char[MAX_ROW_LENGTH_];
  char* line2 = new char[MAX_ROW_LENGTH_];
  char* success = NULL;
  vector<long> edges1;
  vector<long> edges2;
  int partitionIndex = 0;

  long readBlind = minnode_ - oldPartitionNames[0].minnode;
  oldMatrixPartition = openNextPartition(oldPartitionNames[partitionIndex]);
  long blindcount = 0;
  while (blindcount < readBlind) {
  	fgets(line2, MAX_ROW_LENGTH_, oldMatrixPartition);
  	++blindcount;
  }

  long nodeId = minnode_;
  bool endedInOld = false;
	while (true) {
		// This should proceed reading lines from the updated matrix
		// because we may ignore lines after the loop.
		success = fgets(line2, MAX_ROW_LENGTH_, oldMatrixPartition);
		if (NULL == success) {
			fclose(oldMatrixPartition);
			++partitionIndex;
			if (partitionIndex >= (int) oldPartitionNames.size()) {
				endedInOld = true;
				break;
			}


			oldMatrixPartition = openNextPartition(
				oldPartitionNames[partitionIndex]);
		  fgets(line2, MAX_ROW_LENGTH_, oldMatrixPartition);
		}

		success = fgets(line1, MAX_ROW_LENGTH_, updatedMatrix);
		//printf("l1 %sl2 %s", line1, line2);
		if (NULL == success) {
			break;
		}

		Util::readEdges(line1, &edges1);
		Util::readEdges(line2, &edges2);

    storeInUpdatedMatrix(edges1.begin(), edges1.end(),
    		edges2.begin(), edges2.end(), nodeId);
    edges1.clear();
    edges2.clear();
    ++nodeId;
	}

	if (endedInOld) {
		edges2.clear();
		while (NULL != fgets(line1, MAX_ROW_LENGTH_, updatedMatrix)) {
			Util::readEdges(line1, &edges1);
			storeInUpdatedMatrix(edges1.begin(), edges1.end(),
			    		edges2.begin(), edges2.end(), nodeId);
			edges1.clear();
			++nodeId;
		}
	}

	finishMatrix();
	if (!endedInOld) {
	  fclose(oldMatrixPartition);
	}
	fclose(updatedMatrix);
	delete[] line1;
	delete[] line2;
	logger_->info("Reading finished.");
}

void SortedVectorNodeMatrix::finishMatrix() {
	newlyAddedMatrix_->setFinish();
	deletedMatrix_->setFinish();
	unchangedMatrix_->setFinish();
}

FILE* SortedVectorNodeMatrix::openNextPartition(PartitionInfo info) {
  FILE* partition = fopen(info.fname.c_str(), "r");
  return partition;
}

// When containers not set from outside.
void SortedVectorNodeMatrix::initEdgelistContainers() {
	shouldDeleteEdgelistContainers_ = true;
  newlyAddedMatrix_ = new EdgelistContainer;
  unchangedMatrix_ = new EdgelistContainer;
  deletedMatrix_ = new EdgelistContainer;
}

// When edges not set from outside.
void SortedVectorNodeMatrix::initEdgelists() {
	newlyAddedMatrix_->initContainers();
	unchangedMatrix_->initContainers();
	deletedMatrix_->initContainers();
}

void SortedVectorNodeMatrix::storeInUpdatedMatrix(vector<long>::iterator updated,
		vector<long>::iterator updated_end, vector<long>::iterator prev,
		vector<long>::iterator prev_end, long nodeId) {

	while (true) {
		if (updated == updated_end && prev == prev_end) {
			break;
		} else if (updated == updated_end) {
			deletedMatrix_->addEdge(nodeId, *prev++);
			continue;
		} else if (prev == prev_end) {
			newlyAddedMatrix_->addEdge(nodeId, *updated++);
			continue;
		}

		if (*prev < *updated) {
			deletedMatrix_->addEdge(nodeId, *prev);
			++prev;
		} else if (*prev > *updated) {
			newlyAddedMatrix_->addEdge(nodeId, *updated);
			++updated;
		} else {
			unchangedMatrix_->addEdge(nodeId, *prev);
			++prev;
			++updated;
		}
	}
}

void SortedVectorNodeMatrix::flushAll(string prefix) {
	flush(deletedMatrix_, prefix + "old.txt");
	flush(newlyAddedMatrix_, prefix + "updated.txt");
	flush(unchangedMatrix_, prefix + "unchanged.txt");
}

void SortedVectorNodeMatrix::flush(EdgelistContainer* cont, string fName) {
	FILE* f = fopen(fName.c_str(), "w");
	if (NULL == f) {
    logger_->error("Error opening file %s", fName.c_str());
    return;
	}
	cont->flush(f);
}

bool SortedVectorNodeMatrix::isEdgeDeleted(long from, long to) {
	return deletedMatrix_->containsEdge(from, to);
}

