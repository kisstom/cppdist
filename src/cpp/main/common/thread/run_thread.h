#ifndef RUN_THREAD_H_
#define RUN_THREAD_H_

#include "../../algos/algo_components/runnable.h"
#include "thread_manager.h"
#include <log4cpp/Category.hh>

class RunThread : public ThreadManager {
public:
  RunThread(Runnable*);
  protected:
    void main();
  private:
    Runnable* runnable_;
    log4cpp::Category* logger_;
};



#endif  // RUN_THREAD_H_
