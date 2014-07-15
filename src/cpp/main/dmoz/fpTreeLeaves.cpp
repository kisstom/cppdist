/*
 * fpTreeLeaves.cpp
 *
 *  Created on: 2014.07.15.
 *      Author: kisstom
 */

#include "fpTreeLeaves.h"

FpTreeLeaves::FpTreeLeaves() {
  tmpStream = new std::stringstream;
}

FpTreeLeaves::~FpTreeLeaves() {
  delete tmpStream;
}

void FpTreeLeaves::run(FILE* f) {
  char line [1024];
  long root;

  while (fgets(line, 1024, f) != NULL) {
    root = getRoot(line);
    addInnerNodes(line, root);
  }
}

long FpTreeLeaves::getRoot(char * line) {
  long path, fpIndex;

  tmpStream->str("");
  (*tmpStream) << line;
  (*tmpStream) >> fpIndex;
  while (!tmpStream->eof()) {
    (*tmpStream) >> path;
  }

  return path;
}

void FpTreeLeaves::addInnerNodes(char* line, long root) {
  long path, fpIndex;

  tmpStream->str("");
  (*tmpStream) << line;
  (*tmpStream) >> fpIndex;

  while (!tmpStream->eof()) {
    (*tmpStream) >> path;
    if (!tmpStream->eof()) {
      nodes[root].insert(path);
    }
  }
}

