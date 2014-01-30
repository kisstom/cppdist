/*
 * edge_list_builder.cpp
 *
 *  Created on: 2013.11.07.
 *      Author: kisstom
 */

#include "edge_list_builder.h"

EdgeListBuilder::EdgeListBuilder() : fileUtil_(15000000) {
	logger_ = &log4cpp::Category::getInstance(std::string("EdgeListBuilder"));
	//container_->setMinnode(0);
}

void EdgeListBuilder::buildFromFileOld(string fname) {
  FILE* input = fopen(fname.c_str(), "r");
  if (NULL == input) {
  	logger_->error("Could noot open edge list container %s", fname.c_str());
  	return;
  }

  char ** line = fileUtil_.getBufferPointer();
  vector<long> edges;

  long lineNumber = 0;
  while (fileUtil_.readLine(input)) {
  	util_.readEdges(*line, &edges);
  	for (vector<long>::iterator it = edges.begin(); it != edges.end(); ++it) {
  		container_->addEdge(lineNumber, *it);
  	}
    edges.clear();
    ++lineNumber;
    if (lineNumber % 1000000 == 0) {
    	logger_->info("%ld number of rows read from adj matrix.", lineNumber);
    }
  }

  container_->setFinish();
  fclose(input);
}

void EdgeListBuilder::buildFromFileOld2(string fname) {
  FILE* input = fopen(fname.c_str(), "r");
  if (NULL == input) {
  	logger_->error("Could noot open edge list container %s", fname.c_str());
  	return;
  }

  const int ROW_LEN = 15000000;
  char* buffer = new char[ROW_LEN];
  long lineNumber = 0;
  vector<long> edges;

  while (fgets(buffer, ROW_LEN, input)) {
  	util_.readEdges(buffer, &edges);
  	for (vector<long>::iterator it = edges.begin(); it != edges.end(); ++it) {
  		container_->addEdge(lineNumber, *it);
  	}
    edges.clear();
    ++lineNumber;
    if (lineNumber % 1000000 == 0) {
    	logger_->info("%ld number of rows read from adj matrix.", lineNumber);
    }
  }

  container_->setFinish();
  fclose(input);
  delete[] buffer;
}

void EdgeListBuilder::buildFromFile(string fname) {
  FILE* input = fopen(fname.c_str(), "r");
  if (NULL == input) {
  	logger_->error("Could noot open edge list container %s", fname.c_str());
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

  	if (strlen(line) == 1) {
  		++lineNumber;
  		continue;
  	}

  	line[strlen(line)-1] = '\0';
  	split(line, edges);

  	for (vector<long>::iterator it = edges.begin(); it != edges.end(); ++it) {
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

void EdgeListBuilder::split(char* line, vector<long>& edges) {
  edges.clear();
  stringstream ss(stringstream::in | stringstream::out);
  ss << line;
  long edge;

  while (ss.good()) {
    ss >> edge;
    edges.push_back(edge);
  }
}
void EdgeListBuilder::setContainer(EdgelistContainer* container) {
	container_ = container;
}

