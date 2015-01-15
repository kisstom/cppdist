#include "als_evaluator.h"
#include "../../common/graph/adjacency_list_iterator_part.h"
#include "prediction_maker.h"

#include <cmath>

double AlsEvaluator::evaluate() const {
  if (testRatings->getNumberOfEdges() == 0) {
    return 0.0;
  }

  AdjacencyListIteratorPart<Entry> iterator = testRatings->createIteratorPart();
  Entry e;

  double rmse = 0.0, prediction;
  for (long user = 0; user < testRatings->getNumberOfNodes(); ++user) {
    iterator.resetRow(user);

    while (iterator.hasNext()) {
      e = iterator.next();
      prediction = predictionMaker->predict(user, e.id);
      rmse += (e.value - prediction) * (e.value - prediction);
    }
  }

  return sqrt(rmse / testRatings->getNumberOfEdges());
}

void AlsEvaluator::setTestRatings(AdjacencyList<Entry>* _testRatings) {
  testRatings = _testRatings;
}

void AlsEvaluator::setPredictor(PredictionMaker* predictor) {
  predictionMaker = predictor;
}
