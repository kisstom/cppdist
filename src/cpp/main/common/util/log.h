/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 * Copyright 2010 SZTAKI
 */
#ifndef _AEGONLOG_H_
#define _AEGONLOG_H_

#include <sys/time.h>
#include <time.h>
#include <cstdio>
#include <string>
#include <inttypes.h>

enum DEBUG_LEVEL {DEBUG, WARNING, INFO, ERROR};

class Logging {
 public:
  static bool isLoggingOn;
  static char buffer[30];
  static struct timeval tv;
  static time_t curtime;
  static FILE* file;
  static void setTime();
  static void setFile(FILE* _file);
  static void process_mem_usage(double* vm_usage, double* resident_set);
  static DEBUG_LEVEL debug_level;
  static void SetDebugLevel(char * level);
};

#define LOG_ON {Logging::isLoggingOn = true;}
#define LOG_OFF {Logging::isLoggingOn = false;}


#define log_err(fout, format, arg...)  LOG_LLL(fout,  __FILE__, __LINE__, "E", \
    format, ##arg)
#define log_warn(fout, format, arg...)  LOG_LLL(fout, __FILE__ , __LINE__ , "W", \
    format, ##arg)
#define log_info(fout, format, arg...)  LOG_LLL(fout, __FILE__ , __LINE__ , "I", \
    format, ##arg)
#define log_dbg(fout, format, arg...)  LOG_LLL(fout,  __FILE__ , __LINE__ , "D", \
    format, ##arg)


#define err_(format, arg...)  LOG_LLL_DEFAULT(__FILE__ , "E", format, ##arg)
#define warn_(format, arg...)  LOG_LLL_DEFAULT(__FILE__ , "W", format, ##arg)
#define info_(format, arg...)  LOG_LLL_DEFAULT(__FILE__ , "i", format, ##arg)
#define dbg_(format, arg...)  LOG_LLL_DEFAULT(__FILE__ , "d", format, ##arg)


#define LOG_LLL1(fout, domain, levelchr, format, arg...) { \
  if (Logging::isLoggingOn) { \
    time_t rawtime; \
    time(&rawtime); \
    std::string temp = ctime(&rawtime); \
    temp.erase(temp.size() - 1); \
    fprintf(fout, "%s", temp.c_str()); \
    fflush(fout); \
    fprintf(fout, " - " domain "(" levelchr "): " format "\n", ## arg); \
    fflush(fout); \
  } else {} \
}


#define LOG_LLL(fout, domain, line, levelchr, format, arg... ) { \
  DEBUG_LEVEL debug_level;\
  if (!strcmp(levelchr, "E")) { \
         debug_level = ERROR;\
  }\
  else  if (!strcmp(levelchr, "I")) {\
         debug_level = INFO;\
  }\
  else  if (!strcmp(levelchr, "W")) {\
         debug_level = WARNING;\
  }\
  else  if (!strcmp(levelchr, "D")) {\
         debug_level = DEBUG;\
  }\
  if (debug_level >= Logging::debug_level) { \
    Logging::setTime();\
    fprintf(fout, "%s%ld", Logging::buffer, Logging::tv.tv_usec); \
    fflush(fout); \
    fprintf(fout, " - " domain ":%d(" levelchr "): " format "\n", \
        line, ## arg); \
    fflush(fout); \
  } \
}


#define log_mem(fout, format, arg...) { \
  if (Logging::isLoggingOn) { \
    Logging::setTime();\
    double vm, rm; \
    Logging::process_mem_usage(&vm, &rm); \
    fprintf(fout, "%s%ld", Logging::buffer, Logging::tv.tv_usec); \
    fflush(fout); \
    fprintf(fout, " - " __FILE__ ":%d(M): vmem=%f, rmem=%f," format "\n", \
        __LINE__, vm, rm, ##arg); \
    fflush(fout); \
  } \
}


#define LOG_LLL_DEFAULT(domain, levelchr, format, arg... ) { \
  if (Logging::isLoggingOn) { \
    Logging::setTime();\
    fprintf(Logging::file, "%s%ld", Logging::buffer, Logging::tv.tv_usec);  \
    fflush(Logging::file); \
    fprintf(Logging::file, " - " domain "(" levelchr "): " format "\n", \
      ## arg); \
    fflush(Logging::file); \
  } \
}


#endif
