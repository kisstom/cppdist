/*
 * Copyright 2011 SZTAKI
 */

#include "./mutex.h"

Mutex::Mutex() {
  logger_ = &log4cpp::Category::getInstance(std::string("Mutex"));
  pthread_mutex_init(&mutex, NULL);
}

Mutex::~Mutex() {
  pthread_mutex_destroy(&mutex);
}

void Mutex::lock() {
  pthread_mutex_lock(&mutex);
}

void Mutex::unlock() {
  pthread_mutex_unlock(&mutex);
}
