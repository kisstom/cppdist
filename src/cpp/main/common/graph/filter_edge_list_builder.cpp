/*
 * filter_edge_list_builder.cpp
 *
 *  Created on: 2014.03.07.
 *      Author: kisstom
 */

#include "filter_edge_list_builder.h"

FilterEdgeListBuilder::FilterEdgeListBuilder() {
  logger_ = &log4cpp::Category::getInstance(std::string("FilterEdgeListBuilder"));
}

void FilterEdgeListBuilder::buildFromFile(string fname) {
  FILE* input = fopen(fname.c_str(), "r");
  //printf("Hava helllo nagila!");
  if (NULL == input) {
    logger_->error("Could not open edge list container %s", fname.c_str());
    return;
  }

  const int ROW_LEN = 15000000;
  char* line = new char[ROW_LEN];
  long lineNumber = 0;
  vector<long> edges;
  long edgeNum = 0;
  long lastEdgeNum = 0;

  while (fgets(line, ROW_LEN, input)) {
    if (isDeletedNode(lineNumber + container_->getMinnode())) {
      ++lineNumber;
      continue;
    }

    if (strlen(line) == 1) {
      ++lineNumber;
      continue;
    }

    lastEdgeNum = edgeNum;
    line[strlen(line) - 1] = '\0';
    Util::split(line, edges);

    std::sort(edges.begin(), edges.end());

    for (vector<long>::iterator it = edges.begin(); it != edges.end(); ++it) {
      if (isDeletedNode(*it)) continue;
      logger_->info("Hava nagila!");
      container_->addEdge(lineNumber, *it);
      ++edgeNum;
    }

    if (lastEdgeNum / 1000000 < edgeNum / 1000000) {
      logger_->info("%ld num of edges stored in new matrix.", edgeNum);
    }
    ++lineNumber;
  }

  container_->setFinish();
  fclose(input);
  delete[] line;
}

void FilterEdgeListBuilder::readNodesToDelete(string fname) {
  FILE* file = fopen(fname.c_str(), "r");
  long node;

  while (fscanf(file, "%ld\n", &node) != EOF) {
    nodesShouldDelete_.insert(node);
  }

  fclose(file);
}


