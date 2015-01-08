#ifndef ALS_UTIL_H_
#define ALS_UTIL_H_

#include <gsl/gsl_multifit.h>
#include "feature_matrix.h"
#include "../../common/graph/adjacency_list_iterator.h"
#include "../../common/graph/entry.h"

class AlsUtil {
public:
  void setRegressionMatrix(const FeatureMatrix* featMx, AdjacencyListIterator<Entry>* matrixEntries,
      double lambda, gsl_matrix* regrMx);
  void setObjective(AdjacencyListIterator<Entry>* matrixEntries,
      const FeatureMatrix* featMx, gsl_vector *objective);
  double scalarProduct(const FeatureMatrix* featMx,
      AdjacencyListIterator<Entry>* matrixEntries, long colI, long colJ);
  double scalarProduct(AdjacencyListIterator<Entry>* matrixEntries, const FeatureMatrix* featMx,
      int featI);
};

#endif  // ALS_UTIL_H_
