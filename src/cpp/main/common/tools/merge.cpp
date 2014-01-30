#include <stdio.h>
#include "merge.h"

Merge::Merge (vector<string> to_merge, string output) {
  minIndex_.resize(to_merge.size());
  value_.resize(to_merge.size());
  mergeFiles_.resize(to_merge.size());
  isClosed_.resize(to_merge.size(), false);

  for (unsigned i = 0; i < to_merge.size(); ++i) {
    mergeFiles_[i] = fopen(to_merge[i].c_str(), "r");
  }
  outfile_ = fopen(output.c_str(), "w");
  lastPrinted_ = -1;
  numRowPrinted_ = 0;
}

void Merge::close() {
  for (unsigned i = 0; i < mergeFiles_.size(); ++i) {
    fclose(mergeFiles_[i]);
  }
  fclose(outfile_);
  printf("FILE MERGING: %ld row merged.\n", numRowPrinted_);
}

void Merge::merge() {
  init();
  while (hasNext()) {
    printMinIndex();
    readRowLastPrinted();
  }
  close();
}

void Merge::printMinIndex() {
  int i = getIndex();
//  fprintf(outfile_, "%ld %lf\n", minIndex_[i], value_[i]);
  fprintf(outfile_, "%lf\n", value_[i]);
  lastPrinted_ = i;
  ++numRowPrinted_;
}

void Merge::readRowLastPrinted () {
  int nr;
  if (!feof(mergeFiles_[lastPrinted_])) {
    nr = fscanf(mergeFiles_[lastPrinted_], "%ld %lf", &(minIndex_[lastPrinted_]),  &(value_[lastPrinted_]));
    if (nr != 2) isClosed_[lastPrinted_] = true;
  } else {
    isClosed_[lastPrinted_] = true;
  }
}

bool Merge::hasNext() {
  bool ret = false;
  for (unsigned i = 0; i < mergeFiles_.size(); ++i) {
    if (!feof(mergeFiles_[i])) {
      ret = true;
      break;
    }
  }
  return ret;
}

int Merge::getIndex() {
  //long m = minIndex_[0];
  //int index = 0;
  long m = -1;
  int index = -1;
  for (unsigned i = 0; i < mergeFiles_.size(); ++i) {
    if (!isClosed_[i]) {
      if (index == -1) {
         m = minIndex_[i];
         index = i;
      } 
      else if (minIndex_[i] < m) {
         m = minIndex_[i];
         index = i;
      }
    }
  }
  return index;
}

void Merge::init() {
  int nr;
  for (unsigned i = 0; i < mergeFiles_.size(); ++i) {
    if (!feof(mergeFiles_[i])) {
      nr = fscanf(mergeFiles_[i], "%ld %lf", &(minIndex_[i]),  &(value_[i]));
      if (nr != 2) isClosed_[i] = true; 
    } else {
      isClosed_[i] = true;   
    }
  }
}
