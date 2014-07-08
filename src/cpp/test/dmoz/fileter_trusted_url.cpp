#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tr1/unordered_map>

using namespace std;

TEST(test_filter, first) {
  string command("zcat /mnt/ClueWeb09/disk3/WebGraph/ClueWeb09_WG_NodeList_Full.txt.gz|head -10|");
  command.append("/home/kisstom/git/DistributedComp/DistributedFrame/src/bin/main/dmoz/filter_trusted_url ");
  command.append("/home/kisstom/git/DistributedComp/DistributedFrame/src/cpp/test/dmoz/data/test_url ");
  command.append("/home/kisstom/git/DistributedComp/DistributedFrame/src/cpp/test/dmoz/data/id_test_out");
  system(command.c_str());

  tr1::unordered_map<int, string> expected;
  FILE* res = fopen("/home/kisstom/git/DistributedComp/DistributedFrame/src/cpp/test/dmoz/data/id_test_out", "r");
  EXPECT_TRUE(res != NULL);

  char str[1024];
  int id;
  while (fscanf(res, "%s %d\n", str, &id) != EOF) {
    expected[id] = string(str);
  }
  fclose(res);

  EXPECT_TRUE(expected[0].compare("http://del.icio.us/post") == 0);
  EXPECT_TRUE(expected[2].compare("http://www.addthis.com/bookmark.php") == 0);
  EXPECT_TRUE(expected[4].compare("http://www.blogger.com/navbar.g") == 0);
  EXPECT_TRUE(expected[5].compare("http://www.blogger.com/post-edit.g") == 0);
  EXPECT_TRUE(expected[8].compare("http://www.google.com/") == 0);
  EXPECT_TRUE(expected[9].compare("http://www.blogger.com/rearrange") == 0);
}  


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

