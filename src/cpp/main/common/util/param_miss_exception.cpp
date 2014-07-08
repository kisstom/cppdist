/*
 * param_miss_exception.cpp
 *
 *  Created on: 2014.06.18.
 *      Author: kisstom
 */

#include "param_miss_exception.h"

ParamMissException::ParamMissException(string msg) {
  msg_ = msg;
}

const char* ParamMissException::what() const throw()
{
    return msg_.c_str();
}


