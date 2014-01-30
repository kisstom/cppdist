/*
 * test_remove_paralel.cpp
 *
 *  Created on: 2013.11.05.
 *      Author: kisstom
 */

#include "../../../main/common/graph_converter/remove_paralel.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream>

using std::vector;
using std::stringstream;
using std::ios_base;

namespace {

class RemoveParalelTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	RemoveParalelTest() {
  }

  virtual ~RemoveParalelTest() {
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

TEST(RemoveParalelTest, testProcessLine) {
  char line[1024] = "12 43 12 45 12 64\n";
  RemoveParalel remover(1024);
  remover.processLine(line);
  vector<long> processed =  remover.getProcessedLine();
  vector<long> expected;
  expected.push_back(12);
  expected.push_back(43);
  expected.push_back(45);
  expected.push_back(64);
  ASSERT_EQ(expected, processed);

}

TEST(RemoveParalelTestStream, testProcessNext) {
  stringstream ss;
  ss << "12 43 12\n";
  RemoveParalel remover(1024);
  ASSERT_TRUE(remover.processNext(ss));
  vector<long> processed = remover.getProcessedLine();
  vector<long> expected;
  expected.push_back(12);
  ASSERT_EQ(expected, processed);

  ASSERT_TRUE(remover.processNext(ss));
  ASSERT_TRUE(remover.processNext(ss));
  processed = remover.getProcessedLine();
  expected.push_back(43);
  ASSERT_EQ(expected, processed);

  ASSERT_FALSE(remover.processNext(ss));
}

TEST(RemoveParalelTestStream, testProcessNextNoEndline) {
  stringstream ss;
  ss << "12 43 12";
  RemoveParalel remover(1024);
  ASSERT_TRUE(remover.processNext(ss));
  vector<long> processed = remover.getProcessedLine();
  vector<long> expected;
  expected.push_back(12);
  ASSERT_EQ(expected, processed);

  ASSERT_TRUE(remover.processNext(ss));
  ASSERT_TRUE(remover.processNext(ss));
  processed = remover.getProcessedLine();
  expected.push_back(43);
  ASSERT_EQ(expected, processed);

  ASSERT_FALSE(remover.processNext(ss));
}

TEST(RemoveParalelTestStream, testProcessLine) {
	stringstream ss;
	//ss << "12 43 75 43 66\n19 75 43 19 32 43";
	ss << "12 43 75 43 66";
	RemoveParalel remover(1024);
	remover.processLine(ss);
	vector<long> processed = remover.getProcessedLine();
	vector<long> expected;
	expected.push_back(12);
	expected.push_back(43);
	expected.push_back(75);
	expected.push_back(66);
	ASSERT_EQ(expected, processed);
}

TEST(RemoveParalelTestStream, testParseNext) {
	stringstream ss;
	ss << "12 43 75 43 66\n19 75 43 19 32 43";

	RemoveParalel remover(1024);
	stringstream tmpLine(ios_base::in | ios_base::out);

	ASSERT_TRUE(remover.parseNextLine(ss, tmpLine));
	remover.processLine(tmpLine);
	vector<long> processed = remover.getProcessedLine();
	vector<long> expected;
	expected.push_back(12);
	expected.push_back(43);
	expected.push_back(75);
	expected.push_back(66);
	ASSERT_EQ(expected, processed);
	remover.clear(tmpLine);

	ASSERT_TRUE(remover.parseNextLine(ss, tmpLine));
	remover.processLine(tmpLine);
	processed = remover.getProcessedLine();
	expected.clear();
	expected.push_back(19);
	expected.push_back(75);
	expected.push_back(43);
	expected.push_back(32);
	ASSERT_EQ(expected, processed);
	remover.clear(tmpLine);

	ASSERT_FALSE(remover.parseNextLine(ss, tmpLine));
}

TEST(SSTREAM, getline) {
	 stringstream tmpLine(ios_base::in | ios_base::out);
	 tmpLine << "1 2 3 4 5\n6 7 8 9 10 11 12";
	 char buf[15];
	 tmpLine.getline(buf, 15, '\n');
   printf("failbit %d\n", tmpLine.failbit);
 }


/*TEST(RemoveParalelTestStream, testParseMorePiece) {
	stringstream ss;
	ss << "12 43 75 43 66\n19 75 43 19 32 43";

	RemoveParalel remover(5);
	stringstream tmpLine(ios_base::in | ios_base::out);

	ASSERT_TRUE(remover.parseNextLine(ss, tmpLine));
	remover.processLine(tmpLine);
	vector<long> processed = remover.getProcessedLine();
	vector<long> expected;
	expected.push_back(12);
	expected.push_back(43);
	expected.push_back(75);
	expected.push_back(66);
	ASSERT_EQ(expected, processed);
	remover.clear(tmpLine);

	ASSERT_TRUE(remover.parseNextLine(ss, tmpLine));
	remover.processLine(tmpLine);
	processed = remover.getProcessedLine();
	expected.clear();
	expected.push_back(19);
	expected.push_back(75);
	expected.push_back(43);
	expected.push_back(32);
	ASSERT_EQ(expected, processed);
	remover.clear(tmpLine);

	ASSERT_FALSE(remover.parseNextLine(ss, tmpLine));
}*/

TEST(RemoveParalelTestStream, testFail) {
	stringstream ss(ios_base::in | ios_base::out);
	ss << "12 43 75 43 66\n19 75 43 19 32 43";
	char buf[5];
	ss.getline(buf, 5, '\n');
	ASSERT_TRUE(ss.fail());
	std::cout << "fail " << ss.failbit;
	ss.getline(buf, 15, '\n');
	//ASSERT_FALSE(ss.fail());
	std::cout << "fail " << ss.failbit;
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


