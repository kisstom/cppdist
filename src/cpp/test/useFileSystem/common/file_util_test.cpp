/*
 * file_util_test.cpp
 *
 *  Created on: 2013.11.07.
 *      Author: kisstom
 */
#include <gtest/gtest.h>
#include <vector>
#include "../../../main/common/util/file_util.h"
#include <string>
#include "../../common/gtest_environment.h"

using std::vector;
using std::string;

namespace {

class FileUtilTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	FileUtilTest() {
  }

  virtual ~FileUtilTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
  // Objects declared here can be used by all tests in the test case for Foo.
};


TEST(FileUtilTest, readLine) {
  FileUtil fileUtil(10);
  string testMatrix = testResourceDir + string("matrix.txt");

  FILE* f = fopen(testMatrix.c_str(), "r");
  if (NULL == f) {
  	fprintf(stderr, "Error opening file %s", testMatrix.c_str());
  	ASSERT_TRUE(false);
  }

  fileUtil.readLine(f);
  char** buffer = fileUtil.getBufferPointer();
  char expected[1024] = "12 34 0 2 565 2323 565 2 2335 45465 3434\n";
  ASSERT_FALSE(strcmp(expected, *buffer));

  fileUtil.readLine(f);
  char expected2[1024] = "12 34 0 2 565 2323 565 2 2335 45465 343466\n";
  ASSERT_FALSE(strcmp(expected2, *buffer));

  fclose(f);
}

TEST(FileUtilTest, readFingerPrintStart) {
	FileUtil util(1024);
	vector<list<long*> > fpStart;
	string testFingerPrint = testResourceDir + string("fpstarts.txt");

	util.readFingerprintStart((long) 2, (long) 5, 4, &fpStart,
			testFingerPrint, (short) sizeof(long) * 3);

	for (int i = 0; i < (int) fpStart.size(); ++i) {
		for (list<long*>::iterator it = fpStart[i].begin(); it != fpStart[i].end(); ++it) {
			delete[] *it;
		}
	}
}

}

int main (int argc, char **argv) {
	std::string testResourceDir(argc == 2 ? argv[1] : "");
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new MyTestEnvironment(testResourceDir));

  return RUN_ALL_TESTS();
}




