#include "adjacency_list_builder.h"

void AdjacencyListBuilder::buildFromFile(string fname, AdjacencyList<Entry>* container) {
  FILE* input = fopen(fname.c_str(), "r");
  if (NULL == input) {
    logger_->error("Could not open edge list container %s", fname.c_str());
    return;
  }

  long from, to, minNode, lastFromNode;
  double value;

  minNode = container->getMinnode();
  lastFromNode = minNode;
  while (fscanf(input, "%ld %ld %lf\n", &from, &to, &value) != EOF) {
    /*for (long i = lastFromNode; i < from - 1; ++i) {
      container->addSink(i);
    }*/
    container->addEdge(from, Entry(to, value));
    lastFromNode = from;
  }

  container->setFinish();
  fclose(input);
}

void AdjacencyListBuilder::buildTransposeFromFile(string fname, AdjacencyList<Entry>* container) {
  FILE* input = fopen(fname.c_str(), "r");
  if (NULL == input) {
    logger_->error("Could not open edge list container %s", fname.c_str());
    return;
  }

  long from, to, minNode, lastFromNode;
  double value;

  minNode = container->getMinnode();
  lastFromNode = minNode;
  while (fscanf(input, "%ld %ld %lf\n", &to, &from, &value) != EOF) {
    /*for (long i = lastFromNode; i < from - 1; ++i) {
      container->addSink(i);
    }*/
    container->addEdge(from, Entry(to, value));
    lastFromNode = from;
  }

  container->setFinish();
  fclose(input);
}
