/*
 * edgelist_container.cpp
 *
 *  Created on: 2013.07.30.
 *      Author: kisstom
 */

#include <stdio.h>
#include "edgelist_container.h"


EdgelistContainer::EdgelistContainer() {
	minnode_ = -1;
	shouldDeleteContainers_ = false;
	logger_ = &log4cpp::Category::getInstance(std::string("EdgelistContainer"));
}

void EdgelistContainer::setMinnode(long minnode) {
	minnode_ = minnode;
}

bool EdgelistContainer::shouldDeleteContainers() {
	return shouldDeleteContainers_;
}

void EdgelistContainer::initContainers() {
	shouldDeleteContainers_ = true;
	start_edges_ = new vector<long>;
	edge_list_ = new vector<long>;
}

EdgelistContainer::~EdgelistContainer() {
	if (shouldDeleteContainers_) {
		delete start_edges_;
		delete edge_list_;
	}
}

void EdgelistContainer::addEdge(long nodeId, long edge) {
  long nodeIndex = nodeId - minnode_;
  long start = 0;

  while (nodeIndex >= (long) start_edges_->size()) {
    start = (long) edge_list_->size();
  	start_edges_->push_back(start);
  }
  edge_list_->push_back(edge);
}

long EdgelistContainer::neighborhoodSize(long nodeId) {
	long nodeIndex = nodeId - minnode_;
	if (nodeIndex + 1 >= (long) start_edges_->size() || nodeIndex < 0) return -1;

	return start_edges_->at(nodeIndex + 1) - start_edges_->at(nodeIndex);
}

void EdgelistContainer::setFinish() {
	start_edges_->push_back((long) edge_list_->size());
}

long EdgelistContainer::getMinnode() const {
	return minnode_;
}

long EdgelistContainer::getNumberOfNodes() const {
	return start_edges_->size() - 1;
}

long EdgelistContainer::getNumberOfEdges() const {
	return edge_list_->size();
}

void EdgelistContainer::setStartEdges(vector<long>* start_edges) {
	start_edges_ = start_edges;
}

void EdgelistContainer::setEdgeList(vector<long>* edge_list) {
	edge_list_ = edge_list;
}

bool EdgelistContainer::containsEdge(long from, long to) {
	if (from - minnode_ >= getNumberOfNodes() || from - minnode_ < 0) return false;
	int neighborHood = neighborhoodSize(from);
	if (neighborHood == 0) return false;
	bool found = Util::search(to, edge_list_->begin() + start_edges_->at(from - minnode_),
			neighborHood);

	return found;
}


// TODO meg tesztek az edgeListContainer felrecimzesere
long EdgelistContainer::getEdgeAtPos(long node, int index) {
	long nodeIndex = node - minnode_;
	if (nodeIndex < 0) return -1;

  return edge_list_->at(start_edges_->at(nodeIndex) + index);
}

bool EdgelistContainer::operator==(EdgelistContainer& rhs) const {
	if (rhs.getMinnode() != getMinnode()) return false;
	if (rhs.getNumberOfNodes() != getNumberOfNodes()) return false;
	if (rhs.getNumberOfEdges() != getNumberOfEdges()) return false;

	for (int i = 0; i < (int) start_edges_->size(); ++i) {
		if ((*rhs.start_edges_)[i] != (*start_edges_)[i]) return false;
	}

	for (int i = 0; i < (int) edge_list_->size(); ++i) {
		if ((*rhs.edge_list_)[i] != (*edge_list_)[i]) return false;
	}
  return true;
}

void EdgelistContainer::flush(FILE* f = stdout) {
  for (int i = 0; i < (int) start_edges_->size() - 1; ++i) {
    if (start_edges_->at(i) == start_edges_->at(i + 1)) {
      fprintf(f, "\n");
      continue;
    }

    if (start_edges_->at(i) < start_edges_->at(i + 1)) {
      fprintf(f, "%ld", edge_list_->at(start_edges_->at(i)));
    }

  	for (int j = start_edges_->at(i) + 1; j < start_edges_->at(i + 1); ++j) {
  		fprintf(f, " %ld", edge_list_->at(j));
  	}
  	fprintf(f, "\n");
  }
}
