/*
 * crawl_edge_list_builder.cpp
 *
 *  Created on: 2014.03.11.
 *      Author: kisstom
 */

#include "crawl_edge_list_builder.h"

CrawlEdgeListBuilder::CrawlEdgeListBuilder(long maxNodeId) {
  logger_ = &log4cpp::Category::getInstance(string("CrawlEdgeListBuilder"));
  maxNodeId_ = maxNodeId;
}

void CrawlEdgeListBuilder::buildFromFile(string fname) {
  FILE* input = fopen(fname.c_str(), "r");
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
    lastEdgeNum = edgeNum;

    if (lineNumber + container_->getMinnode() > maxNodeId_) {
      break;
    }

    if (strlen(line) == 1) {
      container_->addSinkPart(lineNumber);
      ++lineNumber;
      continue;
    }

    line[strlen(line)-1] = '\0';
    Util::split(line, edges);
    std::sort (edges.begin(), edges.end());

    for (vector<long>::iterator it = edges.begin(); it != edges.end(); ++it) {
      if (*it <= maxNodeId_) {
        container_->addEdgePart(lineNumber, *it);
        ++edgeNum;
      }
    }

    if (lastEdgeNum / 1000000 < edgeNum / 1000000) {
      logger_->info("%ld num of edges stored in new matrix.", edgeNum);
    }
    ++lineNumber;
  }

  logger_->info("%d edges stored.", edgeNum);
  container_->setFinish();
  fclose(input);
  delete[] line;
}


