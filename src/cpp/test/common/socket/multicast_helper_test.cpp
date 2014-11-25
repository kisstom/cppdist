#include <gtest/gtest.h>
#include "../../../main/common/components/socket/multicast_helper.h"
#include <cmath>


TEST(MulticastHelperTest, test1) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);

  short* indices = new short[3];
  // size of the neighbors
  indices[0] = 2;
  indices[1] = 0;
  indices[2] = 3;

  ASSERT_EQ(4, helper.hash(indices));
}

TEST(MulticastHelperTest, test2) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);

  short* indices = new short[3];
  // size of the neighbors
  indices[0] = 2;
  indices[1] = 1;
  indices[2] = 3;

  ASSERT_EQ(3, helper.hash(indices));
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
