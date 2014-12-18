#ifndef TWO_THREADED_MASTER_H_
#define TWO_THREADED_MASTER_H_

#include "master_base.h"

class TwoThreadedMaster: public MasterBase {
public:
  void MakeNodeConnections();
  vector<vector<int> >* MakeTournament();
  void MakeConnection(int i, int j);
};


#endif  // TWO_THREADED_MASTER_H_
