#include "prediction_maker.h"

double PredictionMaker::predict(const long& user, const long& item) const {
  if (userFeats->getRowSize() <= user || itemFeats->getRowSize() <= item ||
      user < 0 || item < 0) return 0.0;

  double retval = 0.0;
  for (int i = 0; i < userFeats->getFeatureSize(); ++i) {
    retval += userFeats->getEntry(user, i) * itemFeats->getEntry(item, i);
  }

  return retval;
}

void PredictionMaker::setFeatures(FeatureMatrix* _userFeats, FeatureMatrix* _itemFeats) {
  userFeats = _userFeats;
  itemFeats = _itemFeats;
}
