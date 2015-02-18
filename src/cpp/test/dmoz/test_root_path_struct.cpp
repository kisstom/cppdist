#include <gtest/gtest.h>
#include <tr1/unordered_map>
#include "../../main/dmoz/root_path_key.h"
#include <functional>
#include <iostream>

TEST(RootPath, test) {
  RootPath k1 = RootPath(5, 10);
  RootPath k2 = RootPath(5, 10);
  RootPath k3 = RootPath(4, 10);

  std::tr1::unordered_map<RootPath, int, RootPathFunc, RootPathFunc> m;
  m[k1] = 1;
  m[k2] = 2;
  m[k3] = 3;

  for (std::tr1::unordered_map<RootPath, int>::iterator it = m.begin(); it != m.end(); ++it) {
    printf("%ld %hd %d\n", it->first.root, it->first.path, it->second);
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
