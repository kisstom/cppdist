
#include <gtest/gtest.h>
#include "../../../main/dmoz/dmoz_reader.h"
#include "../../common/gtest_environment.h"

TEST(DmozReader, all1) {
  DmozReader reader;

  string testContentXml =  testResourceDir + "test_content_xml";

  FILE* input = fopen(testContentXml.c_str(), "r");
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
  std::string testResourceDir(argc == 2 ? argv[1] : "");
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new MyTestEnvironment(testResourceDir));

  return RUN_ALL_TESTS();
}


