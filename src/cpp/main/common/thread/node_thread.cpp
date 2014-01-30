#include <cstring>
#include "./node_thread.h"

NodeThread::NodeThread(Runnable* _runnable, char* msg)
{
  runnable = _runnable;
  strcpy(msg_, msg);
  logger_ = &log4cpp::Category::getInstance(std::string("NodeThread"));
}


// TODO ez igy nagyon nagyon bena
void NodeThread::main()
{
  if (!strcmp(msg_, "sender"))
  {
  	logger_->info("Starting sender.");
  	runnable->sender();
  }
  if (!strcmp(msg_, "receiver"))
  {
  	logger_->info("Starting receiver.");
  	runnable->receiver();
  }
  if (!strcmp(msg_, "setup"))
  {
   	runnable->setUp();
  }
  if (!strcmp(msg_, "run"))
  {
   	runnable->run();
  }
}
