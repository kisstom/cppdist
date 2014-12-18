#ifndef THREE_THREADED_ALGO_H_
#define THREE_THREADED_ALGO_H_

#include "algo_base.h"
#include "../../../common/components/socket/client_socket_manager.h"

class ThreeThreadedAlgo: public AlgoBase {
public:
  ThreeThreadedAlgo();
  bool setUp();
  void run();
  void receiver();
  void runThreads();
  void sendAndSignal(int);
  void setClientSocketManager(ClientSocketManager* clientSocketManager_);

private:
  ClientSocketManager* clientSocketManager_;
};


#endif  // THREE_THREADED_ALGO_H_
