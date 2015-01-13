#include "feature_matrix.h"
#include <stdlib.h>


FeatureMatrix::FeatureMatrix(int _numItems, int _featureSize, double initValue, bool random):
featureSize(_featureSize), numItems(_numItems) {
  entries = new double[numItems * featureSize];
  if (random) {
    srand (1);
  }

  for (int i = 0; i < numItems * featureSize; ++i) {
    if (random) {
      entries[i] = initValue * rand() / RAND_MAX;
    } else {
      entries[i] = initValue;
    }
  }
}

void FeatureMatrix::updateEntry(long item, int feature, const double& value) {
  * (entries + featureSize * item + feature) = value;
}

const double& FeatureMatrix::getEntry(long item, int feature) const {
  return * (entries + featureSize * item + feature);
}

const int& FeatureMatrix::getFeatureSize() const {
  return featureSize;
}

const int& FeatureMatrix::getRowSize() const {
  return numItems;
}

FeatureMatrix::~FeatureMatrix() {
  delete [] entries;
}
