#ifndef NODE_THREAD_H
#define NODE_THREAD_H

#include "../../algos/algo_components/runnable.h"
#include "thread_manager.h"
#include <log4cpp/Category.hh>
#include <exception>
#include <functional>

class NodeThread: public ThreadManager{
  public:
    NodeThread(Runnable* runnable, char* msg);
  protected:
    void main();
  private:
    Runnable* runnable;
    char msg_[1024];
  	log4cpp::Category* logger_;
};

#endif
