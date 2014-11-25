/*
 * test_psimrank_master.cpp
 *
 *  Created on: 2014.06.16.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include "../../../main/algos/algo_components/master.h"
#include "../../../main/algos/algo_components/inner_master.h"
#include "../../../main/algos/psimrank/psimrank_master.h"
#include "../../../main/algos/psimrank/psimrank_random_generator.h"

namespace {

class PSimrankMasterTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  PSimrankMasterTest() {
  }

  virtual ~PSimrankMasterTest() {
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

TEST(PSimrankMasterTest, testInit) {
  Master* master = new Master(0, NULL, 12, false);
  PSimrankMaster* innerMaster = new PSimrankMaster;

  PSimrankRandomGeneratorIFace* randomGen = new PSimrankRandomGenerator(13);
  innerMaster->setRandomGenerator(randomGen);

  innerMaster->setMaster(master);
  innerMaster->init();

  char msg[1024] = "start";
  innerMaster->addInfoForNodes(msg + strlen(msg));
  ASSERT_EQ(0, strcmp(msg, "start"));
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}




