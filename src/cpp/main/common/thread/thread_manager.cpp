/*
 * Copyright 2011 SZTAKI
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./thread_manager.h"
#include "../util/log.h"

void ThreadManager::start() {
  log_dbg(stderr, "ThreadManager::start called.");

  pthread_attr_t attr;

  // Initialize and set thread detached attribute.
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  const int rc = pthread_create(&thread, &attr, static_main,static_cast<void*>(this));
  if (rc) {
    log_err(stderr, "Return code from pthread_create() is %d.", rc);
    exit(-1);
  }

  // Free attribute
  pthread_attr_destroy(&attr);

  log_dbg(stderr, "ThreadManager::start ends for thread %ld.", thread);
}

void* ThreadManager::static_main(void* arg) {
  ThreadManager* threadManager = static_cast<ThreadManager*>(arg);

  log_dbg(stderr, "ThreadManager::static_main called for thread %ld.",
      threadManager->thread);

  threadManager->main();

  log_dbg(stderr, "ThreadManager::static_main ends for thread %ld.",
      threadManager->thread);
  pthread_exit(NULL);
  return 0;
}

void ThreadManager::waitForThread() {
  log_dbg(stderr, "ThreadManager::waitForThread called for thread %ld.",
      thread);

  const int rc = pthread_join(thread, NULL);
  if (rc) {
    log_err(stderr, "Return code from pthread_join() is %d.", rc);
    exit(-1);
  }

  log_dbg(stderr, "ThreadManager::waitForThread ends for thread %ld.",
      thread);
}

ThreadManager::~ThreadManager() {}
