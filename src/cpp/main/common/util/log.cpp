/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 * Copyright 2010 SZTAKI
 */

#include"./log.h"

#include <time.h>

#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

using std::string;

void Logging::process_mem_usage(double* vm_usage, double* resident_set) {
  using std::ios_base;
  using std::ifstream;
  using std::string;

  *vm_usage     = 0.0;
  *resident_set = 0.0;

  // 'file' stat seems to give the most reliable results
  //
  ifstream stat_stream("/proc/self/stat", ios_base::in);

  // dummy vars for leading entries in stat that we don't care about
  //
  string pid, comm, state, ppid, pgrp, session, tty_nr;
  string tpgid, flags, minflt, cminflt, majflt, cmajflt;
  string utime, stime, cutime, cstime, priority, nice;
  string O, itrealvalue, starttime;

  // the two fields we want
  //
  //  unsigned long vsize;
  //  long rss;
  uint64_t vsize;
  int64_t rss;

  // don't care about the rest
  stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
              >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
              >> utime >> stime >> cutime >> cstime >> priority >> nice
              >> O >> itrealvalue >> starttime >> vsize >> rss;

  // in case x86-64 is configured to use 2MB pages
  // long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
  int64_t page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;

  *vm_usage     = vsize / 1024.0;
  *resident_set = rss * page_size_kb;
}


bool Logging::isLoggingOn = true;

char Logging::buffer[30];

struct timeval Logging::tv;

time_t Logging::curtime;

FILE * Logging::file = stderr;

DEBUG_LEVEL Logging::debug_level = INFO;

void Logging::setTime() {
  gettimeofday(&tv, NULL);
  curtime = tv.tv_sec;
  strftime(buffer, 30, "%Y-%m-%d %T.", localtime(&curtime));
}

void Logging::setFile(FILE* _file) {
  file = _file;
}

void Logging::SetDebugLevel(char * level) {
  if (!strcmp(level, "INFO")) {
    debug_level = INFO;
  }
  if (!strcmp(level, "DEBUG")) {
    debug_level = DEBUG;
  }
  if (!strcmp(level, "WARNING")) {
    debug_level = WARNING;
  }
  if (!strcmp(level, "ERROR")) {
    debug_level = ERROR;
  }
  // TODO default
}
