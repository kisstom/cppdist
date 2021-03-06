#ifndef TWO_THREADED_ALGO_H_
#define TWO_THREADED_ALGO_H_

#include "./algo_base.h"

class TwoThreadedAlgo : public AlgoBase {
public:
  TwoThreadedAlgo(unordered_map<string, string>* _params);
  bool setUp();
  void run();
  void receiver();
  void runThreads();
  void sendAndSignal(int);
};


#endif  // TWO_THREADED_ALGO_H_
