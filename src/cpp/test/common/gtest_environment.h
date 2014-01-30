/*
 * gtest_environment.h
 *
 *  Created on: 2014.01.22.
 *      Author: kisstom
 */

#ifndef GTEST_ENVIRONMENT_H_
#define GTEST_ENVIRONMENT_H_

#include <gtest/gtest.h>

namespace {

std::string testResourceDir;

class MyTestEnvironment : public testing::Environment {
 public:
  explicit MyTestEnvironment(const std::string &command_line_arg) {
    testResourceDir = command_line_arg;
  }
};

}


#endif /* GTEST_ENVIRONMENT_H_ */
