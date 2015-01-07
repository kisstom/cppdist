#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../../../main/algos/als/als_util.h"
//#include "../../../main/algos/als/entry.h"

namespace {

TEST(AlsUtilTest, test) {
  Entry e(1, 1.0);
  ASSERT_EQ(e.id, 1);
  ASSERT_DOUBLE_EQ(e.value, 1.0);
}

}
int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
