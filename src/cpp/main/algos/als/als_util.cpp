#include "als_util.h"

void AlsUtil::setRegressionMatrix(const FeatureMatrix* featMx, AdjacencyListIterator<Entry>* matrixEntries,
    double lambda, gsl_matrix* regrMx) {

  double br = 0.0;
  for (int colI = 0; colI < featMx->getFeatureSize(); ++colI) {
    for (int colJ = 0; colJ < featMx->getFeatureSize(); ++colJ) {
      matrixEntries->resetCounter();
      br = scalarProduct(featMx, matrixEntries, colI, colJ);
      gsl_matrix_set (regrMx, colI, colJ, lambda * br);
    }
  }
}

void AlsUtil::setObjective(AdjacencyListIterator<Entry>* matrixEntries,
    const FeatureMatrix* featMx, gsl_vector *objective) {
  double pr = 0.0;
  for (int colI = 0; colI < featMx->getFeatureSize(); ++colI) {
    pr = scalarProduct(matrixEntries, featMx, colI);
    gsl_vector_set(objective, colI, pr);
  }
}

double AlsUtil::scalarProduct(const FeatureMatrix* featMx,
    AdjacencyListIterator<Entry>* matrixEntries, long colI, long colJ) {
  double retval = 0.0;
  long id = 0;
  while (matrixEntries->hasNext()) {
    id = matrixEntries->next().id;
    retval += featMx->getEntry(id, colI) * featMx->getEntry(id, colJ);
  }
  return retval;
}

double AlsUtil::scalarProduct(AdjacencyListIterator<Entry>* matrixEntries, const FeatureMatrix* featMx,
    int featI) {
  double retval = 0.0;
  long id = 0;
  Entry e;

  matrixEntries->resetCounter();
  while (matrixEntries->hasNext()) {
    e = matrixEntries->next();
    retval +=  e.value * featMx->getEntry(e.id, featI);
  }
  return retval;
}
