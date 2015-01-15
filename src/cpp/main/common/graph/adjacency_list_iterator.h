#ifndef ADJACENCY_LIST_ITERATOR_H_
#define ADJACENCY_LIST_ITERATOR_H_

#include "./adjacency_list.h"
#include "log4cpp/Category.hh"
#include "abstract_adjacency_list_iterator.h"

using namespace std;

template<typename T>
class AdjacencyListIterator : public AbstractAdjacencyListIterator<T> {
public:
  AdjacencyListIterator(const AdjacencyList<T>* adjList);
  bool hasNext() const;
  const T& next();
  void resetRow(long);
  void resetCounter();
private:
  long row;
  const AdjacencyList<T>* adjList;
  int pos;
  log4cpp::Category* logger_;
};

template<typename T>
AdjacencyListIterator<T>::AdjacencyListIterator(const AdjacencyList<T>* _adjList):
adjList(_adjList) {
  logger_ = &log4cpp::Category::getInstance(std::string("AdjacencyListIterator"));
  row = -1;
  pos = -1;
}

template<typename T>
bool AdjacencyListIterator<T>::hasNext() const {
  logger_->info("nsize %d", adjList->neighborhoodSize(row));
  return pos + 1 < adjList->neighborhoodSize(row);
}

template<typename T>
const T& AdjacencyListIterator<T>::next() {
  return adjList->getEdgeAtPos(row, ++pos);
}

template<typename T>
void AdjacencyListIterator<T>::resetRow(long _row) {
  row = _row;
  pos = -1;
}

template<typename T>
void AdjacencyListIterator<T>::resetCounter() {
  pos = -1;
}

#endif  // ADJACENCY_LIST_ITERATOR_H_
