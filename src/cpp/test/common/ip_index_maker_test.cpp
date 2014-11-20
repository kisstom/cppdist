#include <gtest/gtest.h>
#include "../../main/common/components/socket/ip_index_maker.h"
#include <cmath>

class TestIPIndexMaker: public ::testing::Test {
public:
  TestIPIndexMaker() {}
};

TEST_F(TestIPIndexMaker, testInit) {
  int nodeIndex = 2;
  int clusterSize = 5;
  IPIndexMaker maker(nodeIndex, clusterSize);

  int expectedLowerBitExcluder = 12;
  int expectedUpperBitExcluder = 3;

  ASSERT_EQ(expectedLowerBitExcluder, maker.lowerBitExcluder);
  ASSERT_EQ(expectedUpperBitExcluder, maker.upperBitExcluder);
}

TEST_F(TestIPIndexMaker, testCreate) {
  int nodeIndex = 2;
  int clusterSize = 5;
  IPIndexMaker maker(nodeIndex, clusterSize);

  int publisherIndex = 0;
  int ipIndex = maker.createPublisherIPIndex(publisherIndex);
  int expectedIndex = 0;
  ASSERT_EQ(expectedIndex, ipIndex);

  publisherIndex = 5;
  ipIndex = maker.createPublisherIPIndex(publisherIndex);
  expectedIndex = 9;
  ASSERT_EQ(expectedIndex, ipIndex);

  int clientIndex = 0;
  ipIndex = maker.createClientIPIndex(clientIndex);
  expectedIndex = 4;
  ASSERT_EQ(expectedIndex, ipIndex);

  clientIndex = 5;
  ipIndex = maker.createClientIPIndex(clientIndex);
  expectedIndex = 13;
  ASSERT_EQ(expectedIndex, ipIndex);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
