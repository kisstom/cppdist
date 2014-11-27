#include <gtest/gtest.h>
#include "../../../main/common/components/socket/multicast_helper.h"
#include <cmath>


TEST(MulticastHelperTest, test1) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);

  short* indices = new short[3];
  bool shouldUpdateSelf = false;
  // size of the neighbors
  indices[0] = 2;
  indices[1] = 0;
  indices[2] = 3;

  ASSERT_EQ(4, helper.publishHashId(indices, &shouldUpdateSelf));
  ASSERT_FALSE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, test2) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  short* indices = new short[3];
  // size of the neighbors
  indices[0] = 2;
  indices[1] = 1;
  indices[2] = 3;

  ASSERT_EQ(3, helper.publishHashId(indices, &shouldUpdateSelf));
  ASSERT_TRUE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, test3) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  short* indices = new short[3];
  // size of the neighbors
  indices[0] = 1;
  indices[1] = 1;

  ASSERT_EQ(-1, helper.publishHashId(indices, &shouldUpdateSelf));
  ASSERT_TRUE(shouldUpdateSelf);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
