/*
 * sorted_vector_node_matrix.h
 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#ifndef SORTED_VECTOR_NODE_MATRIX_H_
#define SORTED_VECTOR_NODE_MATRIX_h_

#include <vector>
#include <string>
#include <stdio.h>
#include <algorithm>
#include "vector_node_matrix.h"
#include "edgelist_container.h"
#include "log4cpp/Category.hh"
#include "../util/util.h"

using std::vector;
using std::string;

struct PartitionInfo {
	string fname;
	long minnode;
	long numnodes;
	long numedges;
};

class SortedVectorNodeMatrix {
public:
	SortedVectorNodeMatrix();
	void storeMatrixes(PartitionInfo info, char* oldSlaveryFile);
	void setMinnode(long);
	void setNumnodes(long);

	EdgelistContainer* getUnchangedMatrix();
	EdgelistContainer* getNewlyAddedMatrix();
	EdgelistContainer* getDeletedMatrix();

	void initEdgelistContainers();
  void initEdgelists();

	//void readUpdatedMatrix();
	void finishMatrix();

	void readUpdatedMatrix(string updatedMatrixFName);
	void readUpdatedMatrix(string updatedMatrixFName,
	  		vector<PartitionInfo> oldPartitionNames);
	PartitionInfo checkThisPartition(char*);
	vector<PartitionInfo> getOldPartitionNames(char*);

  FILE* openNextPartition(PartitionInfo info);
  void storeInUpdatedMatrix(vector<long>::iterator updated,
  		vector<long>::iterator updated_end, vector<long>::iterator prev,
  		vector<long>::iterator prev_end, long nodeId);

  void flush(EdgelistContainer*, string fame);
  void flushAll(string dirName);
  bool isEdgeDeleted(long, long);
  bool shouldDeleteEdgelistContainers();

  ~SortedVectorNodeMatrix();
private:
	long minnode_;
	long numnodes_;
	const int MAX_ROW_LENGTH_;
	EdgelistContainer* unchangedMatrix_;
	EdgelistContainer* newlyAddedMatrix_;
	EdgelistContainer* deletedMatrix_;
	bool shouldDeleteEdgelistContainers_;
	log4cpp::Category* logger_;
};

#endif
