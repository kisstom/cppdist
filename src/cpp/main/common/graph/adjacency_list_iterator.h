#ifndef ADJACENCY_LIST_ITERATOR_H_
#define ADJACENCY_LIST_ITERATOR_H_

#include "./adjacency_list.h"

using namespace std;

template<typename T>
class AdjacencyListIterator {
public:
  AdjacencyListIterator(const AdjacencyList<T>* adjList);
  bool hasNext() const;
  const T next();
  void resetRow(long);
  void resetCounter();
private:
  long row;
  const AdjacencyList<T>* adjList;
  int pos;
};

template<typename T>
AdjacencyListIterator<T>::AdjacencyListIterator(const AdjacencyList<T>* _adjList):
adjList(_adjList) {
  row = -1;
  pos = -1;
}

template<typename T>
bool AdjacencyListIterator<T>::hasNext() const {
  return pos + 1 < adjList->neighborhoodSize(row);
}

template<typename T>
const T AdjacencyListIterator<T>::next() {
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
