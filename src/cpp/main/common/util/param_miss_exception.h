/*
 * param_miss_exception.h
 *
 *  Created on: 2014.06.18.
 *      Author: kisstom
 */

#ifndef PARAM_MISS_EXCEPTION_H_
#define PARAM_MISS_EXCEPTION_H_

#include <exception>
#include <string>

using std::string;

class ParamMissException: public std::exception {
public:
  ParamMissException(string msg);
  const char* what() const throw();
  ~ParamMissException() throw() {}
private:
  string msg_;
};


#endif /* PARAM_MISS_EXCEPTION_H_ */
