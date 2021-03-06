#include "als_util.h"
#include "../../common/graph/adjacency_list.h"
#include "../../common/graph/adjacency_list_iterator_part.h"

AlsUtil::AlsUtil(int _featDim) {
  logger_ = &log4cpp::Category::getInstance(std::string("AlsUtil"));
  featDim = _featDim;
  optimum = gsl_vector_alloc (featDim);
  objective = gsl_vector_alloc (featDim);
  covariance = gsl_matrix_alloc (featDim, featDim);
  regrMx = gsl_matrix_alloc (featDim, featDim);
  work = gsl_multifit_linear_alloc (featDim, featDim);
}

AlsUtil::~AlsUtil() {
  gsl_vector_free(optimum);
  gsl_vector_free(objective);
  gsl_matrix_free(covariance);
  gsl_matrix_free(regrMx);
  gsl_multifit_linear_free(work);
}

void AlsUtil::setRegressionMatrix(const FeatureMatrix* featMx, AbstractAdjacencyListIterator<Entry>* matrixEntries,
    double lambda, gsl_matrix* regrMx) {

  double br = 0.0;
  for (int colI = 0; colI < featMx->getFeatureSize(); ++colI) {
    for (int colJ = 0; colJ < featMx->getFeatureSize(); ++colJ) {
      matrixEntries->resetCounter();
      br = scalarProduct(featMx, matrixEntries, colI, colJ);
      if (colI == colJ) br += lambda;
      gsl_matrix_set (regrMx, colI, colJ, br);
    }
  }
}

void AlsUtil::setObjective(AbstractAdjacencyListIterator<Entry>* matrixEntries,
    const FeatureMatrix* featMx, gsl_vector *objective) {
  double pr = 0.0;
  for (int colI = 0; colI < featMx->getFeatureSize(); ++colI) {
    pr = scalarProduct(matrixEntries, featMx, colI);
    gsl_vector_set(objective, colI, pr);
  }
}

double AlsUtil::scalarProduct(const FeatureMatrix* featMx,
    AbstractAdjacencyListIterator<Entry>* matrixEntries, long colI, long colJ) {
  double retval = 0.0;
  long id = 0;
  while (matrixEntries->hasNext()) {
    id = matrixEntries->next().id;
    retval += featMx->getEntry(id, colI) * featMx->getEntry(id, colJ);
  }
  return retval;
}

double AlsUtil::scalarProduct(AbstractAdjacencyListIterator<Entry>* matrixEntries,
    const FeatureMatrix* featMx, int featI) {
  double retval = 0.0;
  Entry e;

  matrixEntries->resetCounter();
  while (matrixEntries->hasNext()) {
    e = matrixEntries->next();
    retval += e.value * featMx->getEntry(e.id, featI);
  }
  return retval;
}

void AlsUtil::solveOptimisation(const FeatureMatrix* featQ,
    const AdjacencyList<Entry>* mx, long id, double lambda) {
  // TODO ezt innen at lehetne rakni a hivo oldalra
  AdjacencyListIteratorPart<Entry> ratingRow = mx->createIteratorPart();
  ratingRow.resetRow(id);

  setRegressionMatrix(featQ, &ratingRow, lambda, regrMx);
  setObjective(&ratingRow, featQ, objective);

  double chisq = 0.0;
  gsl_multifit_linear (regrMx, objective, optimum, covariance, &chisq, work);

  //logger_->info("Chsq stat %lf", chisq);
}

void AlsUtil::updateFeature(FeatureMatrix* featP, long id) {
  for (int featI = 0; featI < featP->getFeatureSize(); ++featI) {
    featP->updateEntry(id, featI, gsl_vector_get(optimum, featI));
  }
}

double AlsUtil::computeObjective(AdjacencyList<Entry>* ratings,
     const FeatureMatrix* featP, const FeatureMatrix* featQ, double lambda, long id) {
  Entry e;
  double pr = 0.0;
  double obj = 0.0;

  AdjacencyListIteratorPart<Entry> matrixEntries = ratings->createIteratorPart();
  matrixEntries.resetRow(id);

  while (matrixEntries.hasNext()) {
    e = matrixEntries.next();
    pr = 0.0;
    for (int featI = 0; featI < featP->getFeatureSize(); ++featI) {
      pr += featP->getEntry(id, featI) * featQ->getEntry(e.id, featI);
    }

    obj += (e.value - pr) * (e.value - pr);
  }

  double norm = 0.0;
  for (int featI = 0; featI < featP->getFeatureSize(); ++featI) {
    norm += featP->getEntry(id, featI) * featP->getEntry(id, featI);
  }

  obj += norm * lambda;
  return obj;
}




















