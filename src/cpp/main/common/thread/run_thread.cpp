#include "run_thread.h"

RunThread::RunThread(Runnable* runnable) {
  runnable_ = runnable;
  logger_ = &log4cpp::Category::getInstance(std::string("RunThread"));
}

void RunThread::main() {
  runnable_->run();
}
