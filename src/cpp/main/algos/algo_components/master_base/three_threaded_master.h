#ifndef THREE_THREADED_MASTER_H_
#define THREE_THREADED_MASTER_H_

#include "master_base.h"

class ThreeThreadedMaster: public MasterBase {
public:
  ThreeThreadedMaster(int master_port, vector<Slave>* slaves);
  void MakeNodeConnections();
};



#endif  // THREE_THREADED_MASTER_H_
