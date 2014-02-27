/*
 * simrank_test_small.cpp
 *
 *  Created on: 2014.02.24.
 *      Author: kisstom
 */

#include "simrank_test_base.h"

namespace {

class SimrankTestSmall : public SimrankTestBase {
	void initParams() {
		pathLen_ = 10;
		numPathes_ = 2;
		numNodes_ = 0;
		slaveIndex_ = 0;
		slavePort_ = 7001;
		std::stringstream ss;

		ss << slavePort_;
		params_["INIT_SLAVE_PORT"] = ss.str();
		ss.str("");

		ss << pathLen_;
		params_["PATH_LEN"] = ss.str();
		ss.str("");

		ss << numPathes_;
		params_["NUM_PATHES"] = ss.str();
		ss.str("");

		params_["MASTER_PORT"] = "7000";
		params_["SEND_LIMIT"] = "6000";
		params_["MASTER_HOST"] = "localhost";
		params_["NODE_TYPE"] = "SIMRANK_ODD_EVEN";
		params_["RANDOM_TYPE"] = "PSEUDO";
		params_["SEED"] = "13";
		params_["INNER_MASTER_TYPE"] = "SIMRANK_ODD_EVEN";
		params_["DESERIALIZER_TYPE"] = "SIMRANK_ODD_EVEN";
		expectedPathes_.resize(numPathes_);
	}


	virtual void SetUp() {
	    initParams();
	  	initLogger();
	  	vector<string> part1;
	  	vector<string> part2;
	  	part1.push_back("1 2 3");
	  	part1.push_back("");
	  	part1.push_back("9");
	  	part1.push_back("8 10");
	  	part1.push_back("");
	  	part1.push_back("2 4 6");

	  	part2.push_back("");
	  	part2.push_back("2 3 5");
	  	part2.push_back("11");
	  	part2.push_back("");
	  	part2.push_back("");
	  	part2.push_back("1 6 8");

	  	addPartition(part1, 2);
	  	addPartition(part2, 2);

	  	addExpectedPath(0, "1");
	  	addExpectedPath(0, "4");
	  	addExpectedPath(0, "2 9");
	  	addExpectedPath(0, "3 10");
	  	addExpectedPath(0, "5 4");
	  	addExpectedPath(0, "0 2 9");
	  	addExpectedPath(0, "6");
	  	addExpectedPath(0, "9");
	  	addExpectedPath(0, "10");
	  	addExpectedPath(0, "7 3 10");
	  	addExpectedPath(0, "8 11 8 11 6");
	  	addExpectedPath(0, "11 8 11 8 11 6");

	  	addExpectedPath(1, "1");
	  	addExpectedPath(1, "4");
	  	addExpectedPath(1, "2 9");
	  	addExpectedPath(1, "3 10");
	  	addExpectedPath(1, "0 2 9");
	  	addExpectedPath(1, "5 2 9");
	  	addExpectedPath(1, "6");
	  	addExpectedPath(1, "9");
	  	addExpectedPath(1, "10");
	  	addExpectedPath(1, "11 6");
	  	addExpectedPath(1, "7 2 9");
	  	addExpectedPath(1, "8 11 1");

	  	setUpBuilder();
	  	finalSetup();
	}
};

TEST_F(SimrankTestSmall, testRun) {
	Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_);
	cluster.init();
	cluster.start();
	concat(cluster);

  check(concat_, &expectedPathes_);
}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



