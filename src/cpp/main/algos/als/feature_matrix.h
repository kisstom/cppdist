#ifndef FEATURE_MATRIX_H_
#define FEATURE_MATRIX_H_

#include <vector>

using std::vector;

class FeatureMatrix {
public:
  FeatureMatrix(int featureSize, int numItems);
  void updateEntry(long item, int feature, const double& value);
  const double& getEntry(long item, int feature) const;
  const int& getFeatureSize() const;
  ~FeatureMatrix();
private:
  double* entries;
  const int featureSize;
  const int numItems;
};

#endif // FEATURE_MATRIX_H_
