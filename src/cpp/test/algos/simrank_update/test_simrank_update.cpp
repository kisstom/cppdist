#include <gtest/gtest.h>
#include "../../../main/algos/algo.h"
#include "../../../main/algos/master.h"
#include "../../../main/common/thread/node_thread.h"
#include "../../algos/simple_mock_algo/simple_mock_node.h"
#include "../../algos/simple_mock_algo/simple_mock_deserializer.h"
#include "../../algos/simple_mock_algo/simple_mock_inner_master.h"


#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

namespace {

class SimrankUpdateTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SimrankUpdateTest() {
  }

  virtual ~SimrankUpdateTest() {
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

TEST(SimrankUpdateTest, test) {
	/*log4cpp::Appender *appender = new log4cpp::OstreamAppender("console", &std::cout);
	appender->setLayout(new log4cpp::BasicLayout());

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.setPriority(log4cpp::Priority::INFO);
	root.addAppender(appender);

	char localhost[1024] = "localhost";
	char test_cluster1[1024] = "/tmp/test_cluster1";
	char test_cluster2[1024] = "/tmp/test_cluster2";
	char test_input1[1024] = "/tmp/test_input1";
	char test_input2[1024] = "/tmp/test_input2";
  char master_log[1024] = "/tmp/test_master_log";

  unordered_map<string, string>* params = new unordered_map<string, string>;

  (*params)["MASTER_PORT"] = "7000";
  (*params)["INIT_SLAVE_PORT"] = "7001";
  (*params)["SEND_LIMIT"] = "6000";
  (*params)["NUM_SLAVES"] = "3";
  (*params)["NUMLINE"] = 12;
  (*params)["MASTER_HOST"] = "zemif";
  (*params)["LOCAL_DIR"] = "";

  (*params)["SLAVE_INDEX"] = "0";

	char setup[1024] = "setup";
	NodeThread* master_thread = new NodeThread(master, setup);
	NodeThread* algo_thread1 = new NodeThread(algo1, setup);
	NodeThread* algo_thread2 = new NodeThread(algo2, setup);

	root.info("Starting threadded setup.");
	master_thread->start();
	algo_thread1->start();
	algo_thread2->start();

	master_thread->waitForThread();
	algo_thread1->waitForThread();
	algo_thread2->waitForThread();

	root.info("Finished threads.");
	delete master_thread;
	delete algo_thread1;
	delete algo_thread2;

	char run[1024] = "run";
	master_thread = new NodeThread(master, run);
	algo_thread1 = new NodeThread(algo1, run);
	algo_thread2 = new NodeThread(algo2, run);

	root.info("Starting threadded run.");
	master_thread->start();
	algo_thread1->start();
	algo_thread2->start();

	master_thread->waitForThread();
	algo_thread1->waitForThread();
	algo_thread2->waitForThread();

	root.info("Finished threads.");*/
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
