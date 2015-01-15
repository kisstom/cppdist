#ifndef ABSTRACT_ADJACENCY_LIST_ITERATOR_H_
#define ABSTRACT_ADJACENCY_LIST_ITERATOR_H_

template<typename T>
class AbstractAdjacencyListIterator {
public:
  virtual bool hasNext() const = 0;
  virtual const T& next() = 0;
  virtual void resetRow(long) = 0;
  virtual void resetCounter() = 0;
  virtual ~AbstractAdjacencyListIterator() {}
};

#endif  // ABSTRACT_ADJACENCY_LIST_ITERATOR_H_
