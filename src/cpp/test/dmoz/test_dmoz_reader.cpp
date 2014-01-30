#include <gtest/gtest.h>
#include "../../main/dmoz/dmoz_reader.h"

TEST(DmozReader, all1) {
  DmozReader reader;

  FILE* input = fopen("/home/kisstom/git/DistributedComp/DistributedFrame/src/cpp/test/dmoz/data/test_content_xml", "r");
  reader.Init(input);
  reader.Read();
  fclose(input);

  tr1::unordered_map<string, string > url2topic = reader.getUrl2Topic();
  string topic("Top/Arts/Animation");
  string url("http://www.awn.com/");
  string url1("http://www.digitalmediafx.com/Features/animationhistory.html");
  string url2("http://www.animated-divots.net/");
  string url3("http://animation.about.com/");

  EXPECT_TRUE(url2topic[url].compare(topic) == 0);
  EXPECT_TRUE(url2topic[url1].compare(topic) == 0);
  EXPECT_TRUE(url2topic[url2].compare(topic) == 0);
  EXPECT_TRUE(url2topic[url3].compare(topic) == 0);

  string topic2("Top/Arts/Animation/Anime/Characters");
  string url20("http://www.angelfire.com/anime2/ninisbishonen/");
  string url21("http://www.angelfire.com/anime2/bestanimecharacters/");
  string url22("http://valleyofazure.tripod.com/");
  string url23("http://www.angelfire.com/nv/neko/");

  EXPECT_TRUE(url2topic[url20].compare(topic2) == 0);
  EXPECT_TRUE(url2topic[url21].compare(topic2) == 0);
  EXPECT_TRUE(url2topic[url22].compare(topic2) == 0);
  EXPECT_TRUE(url2topic[url23].compare(topic2) == 0);

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


