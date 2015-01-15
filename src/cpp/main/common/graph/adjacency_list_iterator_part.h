#ifndef ADJACENCY_LIST_ITERATOR_PART_H_
#define ADJACENCY_LIST_ITERATOR_PART_H_

#include "./adjacency_list.h"
#include "log4cpp/Category.hh"
#include "abstract_adjacency_list_iterator.h"

using namespace std;

template<typename T>
class AdjacencyListIteratorPart : public AbstractAdjacencyListIterator<T> {
public:
  AdjacencyListIteratorPart(const AdjacencyList<T>* adjList);
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
AdjacencyListIteratorPart<T>::AdjacencyListIteratorPart(const AdjacencyList<T>* _adjList):
adjList(_adjList) {
  logger_ = &log4cpp::Category::getInstance(std::string("AdjacencyListIteratorPart"));
  row = -1;
  pos = -1;
}

template<typename T>
bool AdjacencyListIteratorPart<T>::hasNext() const {
  return pos + 1 < adjList->neighborhoodSizePart(row);
}

template<typename T>
const T& AdjacencyListIteratorPart<T>::next() {
  return adjList->getEdgeAtPosPart(row, ++pos);
}

template<typename T>
void AdjacencyListIteratorPart<T>::resetRow(long _row) {
  row = _row;
  pos = -1;
}

template<typename T>
void AdjacencyListIteratorPart<T>::resetCounter() {
  pos = -1;
}

#endif  // ADJACENCY_LIST_ITERATOR_PART_H_
