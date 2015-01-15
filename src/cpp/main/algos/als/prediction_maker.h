#ifndef PREDICTION_MAKER_H_
#define PREDICTION_MAKER_H_

#include "feature_matrix.h"

class PredictionMaker {
public:
  double predict(const long& user, const long& item) const;
  void setFeatures(FeatureMatrix*, FeatureMatrix*);
private:
  FeatureMatrix* userFeats;
  FeatureMatrix* itemFeats;
};


#endif  // PREDICTION_MAKER_H_
