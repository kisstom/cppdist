#ifndef ALS_UTIL_H_
#define ALS_UTIL_H_

#include <gsl/gsl_multifit.h>
#include "feature_matrix.h"
#include "../../common/graph/abstract_adjacency_list_iterator.h"
#include "../../common/graph/adjacency_list.h"
#include "../../common/graph/entry.h"
#include "log4cpp/Category.hh"

class AlsUtil {
public:
  AlsUtil(int featDim);

  void setRegressionMatrix(const FeatureMatrix* featMx, AbstractAdjacencyListIterator<Entry>* matrixEntries,
      double lambda, gsl_matrix* regrMx);
  void setObjective(AbstractAdjacencyListIterator<Entry>* matrixEntries,
      const FeatureMatrix* featMx, gsl_vector *objective);

  double scalarProduct(const FeatureMatrix* featMx,
      AbstractAdjacencyListIterator<Entry>* matrixEntries, long colI, long colJ);
  double scalarProduct(AbstractAdjacencyListIterator<Entry>* matrixEntries, const FeatureMatrix* featMx,
      int featI);

  void solveOptimisation(const FeatureMatrix* featQ,
      const AdjacencyList<Entry>* mx, long id, double lambda);

  void updateFeature(FeatureMatrix* featP, long id);

  double computeObjective(AdjacencyList<Entry>* ratings,
      const FeatureMatrix* featP, const FeatureMatrix* featQ, double lambda, long id);

  ~AlsUtil();
private:
  gsl_matrix* covariance;
  gsl_matrix* regrMx;
  gsl_vector* optimum;
  gsl_vector* objective;
  gsl_multifit_linear_workspace * work;
  int featDim;
  log4cpp::Category* logger_;
};

#endif  // ALS_UTIL_H_
