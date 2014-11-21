/*
 * simrank_test_small.cpp
 *
 *  Created on: 2014.02.24.
 *      Author: kisstom
 */

#include "simrank_test_base.h"

namespace {

class SimrankTestSmall : public SimrankTestBase {
protected:
	virtual void SetUp() {
	    initParams("SIMRANK_ODD_EVEN");
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

	void concat(Cluster& cluster) {
	  concat_ = new vector<vector<long*> >;
	  concat_->resize(numPathes_);

	  SimrankOddEvenNode* node;
	  vector<vector<long*> >* finished;
	  vector<list<long*> >* pathes;

	  for (int i = 0; i < slaveIndex_; ++i) {
	    node = static_cast<SimrankOddEvenNode*>(cluster.getNode(i));
	    finished = node->getFinishedPathes();
	    for (int i = 0; i < (int) finished->size(); ++i) {

	      for (vector<long*>::iterator it = (*finished)[i].begin();
	          it != (*finished)[i].end(); ++it) {
	        (*concat_)[i].push_back(*it);
	      }

	    }
	    vector<list<long*> >* pathes = node->getPathes();
	    for (int i = 0; i < (int) pathes->size(); ++i) {

	      for (list<long*>::iterator it = (*pathes)[i].begin();
	          it != (*pathes)[i].end(); ++it) {
	        (*concat_)[i].push_back(*it);
	      }

	    }
	  }

	}

	void initParams(string nodeType) {
	  numPathes_ = 2;
	  pathLen_ = 10;
	  SimrankTestBase::initParams(nodeType);
	}
};

TEST_F(SimrankTestSmall, testRun) {
	Cluster cluster(&params_, &nodeParams_, nodeFactories_, masterBuilder_, &clusterNodeParams);
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



