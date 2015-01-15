#ifndef ALS_EVALUATOR_H_
#define ALS_EVALUATOR_H_

#include "../../common/graph/entry.h"
#include "../../common/graph/adjacency_list.h"

class PredictionMaker;

class AlsEvaluator {
public:
  double evaluate() const;
  void setPredictor(PredictionMaker*);
  void setTestRatings(AdjacencyList<Entry>*);
private:
  AdjacencyList<Entry>* testRatings;
  PredictionMaker* predictionMaker;
};


#endif  // ALS_EVALUATOR_H_
