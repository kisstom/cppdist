/*
 * simrank_odd_even_test.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include <gtest/gtest.h>

#include "../../../main/algos/cluster.h"

using std::tr1::unordered_map;

namespace {

class SimrankOddEvenTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SimrankOddEvenTest() {
  }

  virtual ~SimrankOddEvenTest() {
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

TEST(SimrankOddEvenTest, testRun) {
	unordered_map<string, string> params;

	params["MASTER_PORT"] = "7000";
	params["INIT_SLAVE_PORT"] = "7001";
	params["SEND_LIMIT"] = "6000";
	params["NUM_SLAVES"] = "3";
	// numline variable
	params["NUMLINE"] = "3";
	params["MASTER_HOST"] = "localhost";
	//params["LOCAL_DIR"] = "/tmp/simrank_odd_even_test/";
	params["NODE_TYPE"] = "SIMRANK_ODD_EVEN";
	params["NUM_PATHES"] = "10";
	params["PATH_LEN"] = "10";
	params["RANDOM_TYPE"] = "PSEUDO";
	params["SEED"] = "13";
	//params["FP_START_NAME"] = "NULL";
	//params["LOGFILE_NAME"] = "NULL";
	//params["REMOTE_DIR"] = "NULL";
	//params["SLAVERY_CFG"] = "slavery.cfg";
	params["INNER_MASTER_TYPE"] = "SIMRANK_ODD_EVEN";
	params["APPENDER"] = "CONSOLE";

	Cluster cluster(&params);
	cluster.init();
	cluster.start();
}
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


