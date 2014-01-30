/*
 * Copyright 2011 SZTAKI
 */

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <log4cpp/Category.hh>
#include <string>

using std::string;

// Wrapper class for pthread_mutex.
class Mutex {
 private:
  pthread_mutex_t mutex;
  log4cpp::Category* logger_;
 public:
  // Initially unlocked.
  Mutex();
  ~Mutex();
  void lock();
  void unlock();
};

#endif    // MUTEX_H
