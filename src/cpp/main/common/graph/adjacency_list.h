#ifndef ADJACENCY_LIST_H_
#define ADJACENCY_LIST_H_

#include <vector>
#include "../util/util.h"
#include "log4cpp/Category.hh"

using std::vector;

template<class T>
class AdjacencyList {
public:
  AdjacencyList();
  virtual ~AdjacencyList();
  void setStartEdges(vector<long>*);
  void setEdgeList(vector<T>*);
  void addEdge(long key, T entry);
  void addSink(long key);

  void addSinkPart(long key);
  void addEdgePart(long key, T entry);

  void setMinnode(long);
  bool shouldDeleteContainers();
  void initContainers();
  void setFinish();

  bool containsEdge(long, T entry);
  bool operator==(AdjacencyList& rhs) const;
  //virtual void flush(FILE* f) = 0;

  long neighborhoodSize(long key);
  long neighborhoodSizePart(long key);
  T getEdgeAtPos(long, int);
  T getEdgeAtPosPart(long, int);
  long getMinnode() const;
  long getNumberOfNodes() const;
  long getNumberOfEdges() const;
private:
  vector<long>* start_edges_;
  vector<T>* edge_list_;
  long minnode_;
  bool shouldDeleteContainers_;
  log4cpp::Category* logger_;
};

template<class  T>
bool AdjacencyList<T>::shouldDeleteContainers() {
  return shouldDeleteContainers_;
}

template<class  T>
AdjacencyList<T>::AdjacencyList() {
  minnode_ = -1;
  shouldDeleteContainers_ = false;
  logger_ = &log4cpp::Category::getInstance(std::string("AdjacencyList"));
  start_edges_ = NULL;
  edge_list_ = NULL;
}

template <class T>
void AdjacencyList<T>::setMinnode(long minnode) {
  minnode_ = minnode;
}

template <class T>
void AdjacencyList<T>::initContainers() {
  shouldDeleteContainers_ = true;
  start_edges_ = new vector<long>;
  edge_list_ = new vector<long>;
}

template <class T>
AdjacencyList<T>::~AdjacencyList() {
  if (shouldDeleteContainers_) {
    delete start_edges_;
    delete edge_list_;
  }
}

template <class T>
void AdjacencyList<T>::addEdgePart(long nodeId, T edge) {
  long start = 0;

  while (nodeId >= (long) start_edges_->size()) {
    start = (long) edge_list_->size();
    start_edges_->push_back(start);
  }
  edge_list_->push_back(edge);
}

template <class T>
void AdjacencyList<T>::addSinkPart(long nodeId) {
  long start = 0;

  while (nodeId >= (long) start_edges_->size()) {
    start = (long) edge_list_->size();
    start_edges_->push_back(start);
  }
}

template <class T>
void AdjacencyList<T>::addSink(long nodeId) {
  long nodeIndex = nodeId - minnode_;
  long start = 0;

  while (nodeIndex >= (long) start_edges_->size()) {
    start = (long) edge_list_->size();
    start_edges_->push_back(start);
  }
}

template <class T>
void AdjacencyList<T>::addEdge(long nodeId, T edge) {
  long nodeIndex = nodeId - minnode_;
  long start = 0;

  while (nodeIndex >= (long) start_edges_->size()) {
    start = (long) edge_list_->size();
    start_edges_->push_back(start);
  }
  edge_list_->push_back(edge);
}

template <class T>
long AdjacencyList<T>::neighborhoodSize(long nodeId) {
  long nodeIndex = nodeId - minnode_;
  if (nodeIndex + 1 >= (long) start_edges_->size() || nodeIndex < 0) return -1;

  return start_edges_->at(nodeIndex + 1) - start_edges_->at(nodeIndex);
}

template <class T>
long AdjacencyList<T>::neighborhoodSizePart(long nodeId) {
  if (nodeId + 1 >= (long) start_edges_->size() || nodeId < 0) return -1;

  return start_edges_->at(nodeId + 1) - start_edges_->at(nodeId);
}

template <class T>
void AdjacencyList<T>::setFinish() {
  start_edges_->push_back((long) edge_list_->size());
}

template <class T>
long AdjacencyList<T>::getMinnode() const {
  return minnode_;
}

template <class T>
long AdjacencyList<T>::getNumberOfNodes() const {
  return start_edges_->size() - 1;
}

template <class T>
long AdjacencyList<T>::getNumberOfEdges() const {
  return edge_list_->size();
}

template <class T>
void AdjacencyList<T>::setStartEdges(vector<long>* start_edges) {
  start_edges_ = start_edges;
}

template <class T>
void AdjacencyList<T>::setEdgeList(vector<T>* edge_list) {
  edge_list_ = edge_list;
}

template <class T>
bool AdjacencyList<T>::containsEdge(long from, T to) {
  if (from - minnode_ >= getNumberOfNodes() || from - minnode_ < 0) return false;
  int neighborHood = neighborhoodSize(from);
  if (neighborHood == 0) return false;
  bool found = Util::search(to, edge_list_->begin() + start_edges_->at(from - minnode_),
      neighborHood);

  return found;
}

template <class T>
T AdjacencyList<T>::getEdgeAtPos(long node, int index) {
  long nodeIndex = node - minnode_;
  if (nodeIndex < 0) return -1;

  return edge_list_->at(start_edges_->at(nodeIndex) + index);
}

template <class T>
T AdjacencyList<T>::getEdgeAtPosPart(long node, int index) {
  if (node < 0) return -1;

  return edge_list_->at(start_edges_->at(node) + index);
}

template <class T>
bool AdjacencyList<T>::operator==(AdjacencyList& rhs) const {
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

#endif  // ADJACENCY_LIST_H_
