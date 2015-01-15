#include "../../algos/als/als_evaluator.h"
#include "../graph/builder/adjacency_list_builder.h"
#include "../../algos/als/feature_matrix.h"
#include "../../algos/als/prediction_maker.h"
#include <cstdlib>

int main(int argc, char* argv[]) {
  long numUsers = atol(argv[4]);
  long numItems = atol(argv[5]);
  int numFeats = atoi(argv[6]);

  AdjacencyList<Entry> testRatings;
  FeatureMatrix userFeats(numUsers, numFeats);
  FeatureMatrix itemFeats(numItems, numFeats);

  testRatings.initContainers();
  testRatings.setMinnode(0);
  AdjacencyListBuilder builder;
  builder.buildFromFile(string(argv[1]), &testRatings);

  userFeats.loadFromFile(string(argv[2]));
  itemFeats.loadFromFile(string(argv[3]));

  PredictionMaker predictionMaker;
  AlsEvaluator alsEvaluator;

  predictionMaker.setFeatures(&userFeats, &itemFeats);
  alsEvaluator.setTestRatings(&testRatings);
  alsEvaluator.setPredictor(&predictionMaker);

  double eval = alsEvaluator.evaluate();
  printf("rmse %lf\n", eval);
}
