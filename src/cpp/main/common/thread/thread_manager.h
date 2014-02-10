/*
 * Copyright 2011 SZTAKI
 */

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <pthread.h>
#include <log4cpp/Category.hh>

enum pExit {
  PE_CONT,
  PE_ABORT,
  PE_RESTART,
  PE_SOFT_RESTART
};



// Wrapper class for pthread.
// Can be used to securely launch and join a thread.
class ThreadManager {
 private:
  pthread_t thread;

  // This is called as the new thread.
  static void* static_main(void* arg);
  log4cpp::Category* logger_;
 protected:
  // Overwrite this to implement your functionality.
  // It will be run in a separate thread.
  virtual void main() = 0;

 public:
  // Call this to start the thread.
  ThreadManager();
  void start();

  // Blocks the caller thread until the started thread terminates.
  // Only the thread that called start can call it after the start!
  void waitForThread();

  virtual ~ThreadManager();
};

#endif    // THREADMANAGER_H
