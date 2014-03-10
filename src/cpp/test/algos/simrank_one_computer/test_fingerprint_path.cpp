/*
 * test_fingerprint_path.cpp
 *
 *  Created on: 2014.02.10.
 *      Author: mrita
 */

//#define private public

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../../../main/algos/simrank_one_computer/fingerprint_path_vector.h"
#include "../../../main/common/random/hash_pseudo_random.h"
#include "../../../main/common/graph/edgelist_container.h"

using std::vector;
using std::string;

namespace {

class FingerprintPathTest: public ::testing::Test  {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	FingerprintPathTest() {
  }

  virtual ~FingerprintPathTest() {
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

/* 
TEST(FingerprintPathTest, testgetRandomNeighbor) {
  EdgelistContainer* container = new EdgelistContainer();
  container->setMinnode(0);
  container->initContainers();
  container->addEdge(2, 0);
  container->addEdge(2, 1);
  container->addEdge(2, 3);
  container->setFinish();

  HashPseudoRandom* random = new HashPseudoRandom(13);

  FingerprintPathVector fpvect(13, container, random);
  ASSERT_EQ(-1, fpvect.getRandomNeighbor(1,0,0));
  ASSERT_EQ(3, fpvect.getRandomNeighbor(2,0,1));
  ASSERT_EQ(1, fpvect.getRandomNeighbor(2,1,0));
  ASSERT_EQ(3, fpvect.getRandomNeighbor(2,0,0));
  ASSERT_EQ(1, fpvect.getRandomNeighbor(2,5,6));

}
*/

TEST(FingerprintPathTest, testmake) {
  
  EdgelistContainer* container = new EdgelistContainer();
  container->setMinnode(0);
  container->initContainers();
  container->addEdgeWithMinnode(2, 0);
  container->addEdgeWithMinnode(2, 1);
  container->addEdgeWithMinnode(2, 4);
  container->addEdgeWithMinnode(3, 0);
  container->addEdgeWithMinnode(4, 0);
  container->setFinish();

 
  HashPseudoRandom* random = new HashPseudoRandom(13);

  FingerprintPathVector fpvect(13, container, random);
  fpvect.init();
 
  fpvect.make(1,0,4);
  fpvect.make(2,0,4);
  fpvect.make(2,1,4);
  fpvect.make(2,10,4);
  fpvect.make(3,7,4);

  vector<string> vektor;
  vektor.push_back("0 1");
  vektor.push_back("0 2 4 0");
  vektor.push_back("1 2 1"); 
  vektor.push_back("10 2 0");
  vektor.push_back( "7 3 0"); 

  ASSERT_TRUE(vektor == *(fpvect.getVector()));

}

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
