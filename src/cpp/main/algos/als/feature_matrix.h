#ifndef FEATURE_MATRIX_H_
#define FEATURE_MATRIX_H_

#include <vector>
#include <string>
#include "log4cpp/Category.hh"

using std::vector;
using std::string;

class FeatureMatrix {
public:
  FeatureMatrix(int numItems, int featureSize, double initValue = 0.0, bool random = false);
  void updateEntry(long item, int feature, const double& value);
  const double& getEntry(long item, int feature) const;
  const int& getFeatureSize() const;
  const int& getRowSize() const;
  void loadFromFile(string);
  ~FeatureMatrix();
private:
  log4cpp::Category* logger_;
  double* entries;
  const int featureSize;
  const int numItems;
};

#endif // FEATURE_MATRIX_H_
