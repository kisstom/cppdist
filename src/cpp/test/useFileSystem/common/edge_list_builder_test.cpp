/*
 * edge_list_builder_test.cpp
 *
 *  Created on: 2013.11.08.
 *      Author: kisstom
 */

#include <gtest/gtest.h>
#include <vector>
#include "../../../main/common/graph/edge_list_builder.h"
#include <string>
#include "../../common/gtest_environment.h"

using std::vector;
using std::string;

namespace {

/*std::string testResourceDir;

class MyTestEnvironment : public testing::Environment {
 public:
  explicit MyTestEnvironment(const std::string &command_line_arg) {
    testResourceDir = command_line_arg;
  }
};*/

class EdgeListBuilderTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	EdgeListBuilderTest()  {
  }

  virtual ~EdgeListBuilderTest() {
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

  string testDataDir_;
  //const string path_[1024];
  // Objects declared here can be used by all tests in the test case for Foo.
};
}

TEST(EdgeListBuilderTest, readMatrix) {
	EdgeListBuilder builder;
	EdgelistContainer* cont = new EdgelistContainer();
	cont->initContainers();
	cont->setMinnode(0);
	builder.setContainer(cont);

	string matrix = testResourceDir + string("/matrix.txt");
	builder.buildFromFile(matrix.c_str());
	delete cont;
}

TEST(EdgeListBuilderTest, readMatrix2) {
	EdgeListBuilder builder;
	EdgelistContainer* cont = new EdgelistContainer();
	cont->initContainers();
	cont->setMinnode(0);
	builder.setContainer(cont);

	string file = testResourceDir + string("/matrix2");
	builder.buildFromFile(file);

	EdgelistContainer* expected = new EdgelistContainer();
	expected->initContainers();
	expected->setMinnode(0);

	expected->addEdgeWithMinnode(0, 1);
	expected->addEdgeWithMinnode(0, 2);
	expected->addEdgeWithMinnode(0, 3);
	expected->addEdgeWithMinnode(0, 4);
	expected->addEdgeWithMinnode(1, 5);
	expected->addEdgeWithMinnode(1, 6);

	expected->addEdgeWithMinnode(4, 7);
	expected->addEdgeWithMinnode(4, 8);
	expected->addEdgeWithMinnode(5, 9);
	expected->addEdgeWithMinnode(5, 11);
	expected->addEdgeWithMinnode(6, 23);
	expected->addEdgeWithMinnode(6, 45);

	expected->setFinish();
	//cont->flush(stdout);
	//expected->flush(stdout);
	ASSERT_TRUE(*expected == *cont);

	delete cont;
	delete expected;
}



int main (int argc, char **argv) {
	std::string testResourceDir(argc == 2 ? argv[1] : "");
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new MyTestEnvironment(testResourceDir));

  return RUN_ALL_TESTS();
}


