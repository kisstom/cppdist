#include "feature_matrix.h"

FeatureMatrix::FeatureMatrix(int _featureSize, int _numItems):
featureSize(_featureSize), numItems(_numItems) {
  entries = new double[numItems * featureSize];
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

FeatureMatrix::~FeatureMatrix() {
  delete [] entries;
}
